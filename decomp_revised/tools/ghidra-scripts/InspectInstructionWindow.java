// Print instructions around selected addresses, including predecessors of an
// address that Ghidra has not assigned to a function.
// Usage: -postScript InspectInstructionWindow.java 00462039 [00462457 ...]
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import java.util.ArrayDeque;

public class InspectInstructionWindow extends GhidraScript {
    @Override
    public void run() throws Exception {
        for (String raw : getScriptArgs()) {
            Address target = toAddr(raw.startsWith("0x") ? raw : "0x" + raw);
            println("--- WINDOW " + target + " ---");
            InstructionIterator iterator = currentProgram.getListing().getInstructions(
                target.subtract(0x400), true);
            ArrayDeque<Instruction> before = new ArrayDeque<>();
            Instruction match = null;
            while (iterator.hasNext()) {
                Instruction instruction = iterator.next();
                if (instruction.getAddress().equals(target)) {
                    match = instruction;
                    break;
                }
                if (instruction.getAddress().compareTo(target) >= 0) break;
                before.addLast(instruction);
                if (before.size() > 48) before.removeFirst();
            }
            if (match == null) {
                println("No instruction boundary at address; closest preceding instructions:");
                for (Instruction instruction : before) println(instruction.getAddress() + "  " + instruction);
                continue;
            }
            for (Instruction instruction : before) println(instruction.getAddress() + "  " + instruction);
            while (match != null) {
                println(match.getAddress() + "  " + match);
                if (match.getFlowType().isTerminal()) break;
                match = currentProgram.getListing().getInstructionAfter(match.getAddress());
                if (match == null || match.getAddress().compareTo(target.add(0x180)) > 0) break;
            }
        }
    }
}
