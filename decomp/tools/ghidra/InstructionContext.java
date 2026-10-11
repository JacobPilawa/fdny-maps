import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;
import java.util.ArrayList;
import java.util.List;

/** Print an instruction window around an explicit address, even without function boundaries. */
public class InstructionContext extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] requested = getScriptArgs();
        if (requested.length == 0 || requested.length > 3) {
            println("Usage: InstructionContext.java <address> [before-count] [after-count]");
            return;
        }

        String text = requested[0];
        Address center = toAddr(text.startsWith("0x") || text.startsWith("0X")
            ? text.substring(2) : text);
        int beforeCount = requested.length > 1 ? Integer.parseInt(requested[1]) : 12;
        int afterCount = requested.length > 2 ? Integer.parseInt(requested[2]) : 12;
        Listing listing = currentProgram.getListing();
        Instruction centerInstruction = listing.getInstructionAt(center);
        if (centerInstruction == null) {
            println("NO INSTRUCTION at " + center);
            return;
        }

        Function function = getFunctionContaining(center);
        println("ADDRESS " + center + " FUNCTION " + (function == null ? "<none>"
            : function.getEntryPoint() + " " + function.getName()));

        List<Instruction> previous = new ArrayList<>();
        Address cursor = center;
        for (int i = 0; i < beforeCount; i++) {
            Instruction instruction = listing.getInstructionBefore(cursor);
            if (instruction == null) break;
            previous.add(0, instruction);
            cursor = instruction.getAddress();
        }
        for (Instruction instruction : previous) println("  " + instruction);
        println("> " + centerInstruction);
        cursor = centerInstruction.getAddress();
        for (int i = 0; i < afterCount; i++) {
            Instruction instruction = listing.getInstructionAfter(cursor);
            if (instruction == null) break;
            println("  " + instruction);
            cursor = instruction.getAddress();
        }
    }
}
