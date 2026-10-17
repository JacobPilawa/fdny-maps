// Print decoded instructions and any containing function for a code address.
// Usage: -postScript InspectCodeAtAddress.java 00430df0
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;

public class InspectCodeAtAddress extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) throw new IllegalArgumentException("Pass one or more code addresses");
        for (String argument : args) {
            Address target = toAddr(argument.startsWith("0x") ? argument : "0x" + argument);
            Function function = getFunctionContaining(target);
            println("--- CODE " + target + " FUNCTION " +
                (function == null ? "<unmapped>" : function.getEntryPoint() + " " + function.getName()) + " ---");
            Instruction instruction = currentProgram.getListing().getInstructionAt(target);
            if (instruction == null) {
                println("No instruction boundary is defined at the requested address.");
                continue;
            }
            for (int count = 0; instruction != null && count < 160; count++) {
                println(instruction.getAddress() + "  " + instruction);
                if (instruction.getFlowType().isTerminal()) break;
                instruction = currentProgram.getListing().getInstructionAfter(instruction.getAddress());
            }
        }
    }
}
