import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;

/** Find literal fixed-offset indirect CALL/JMP operands, for example [EDX + 0x14]. */
public class FindIndirectCallSlot extends GhidraScript {
    @Override
    public void run() throws Exception {
        String needle = getScriptArgs().length > 0 ? getScriptArgs()[0] : "+ 0x14";
        Listing listing = currentProgram.getListing();
        int count = 0;
        for (Instruction instruction : listing.getInstructions(true)) {
            String text = instruction.toString();
            String mnemonic = instruction.getMnemonicString();
            if ((mnemonic.equalsIgnoreCase("CALL") || mnemonic.equalsIgnoreCase("JMP"))
                    && text.contains(needle)) {
                Function function = getFunctionContaining(instruction.getAddress());
                println(instruction.getAddress() + " " + text + " function="
                    + (function == null ? "<none>"
                        : function.getEntryPoint() + " " + function.getName()));
                count++;
            }
        }
        println("MATCH_COUNT " + count);
    }
}
