// Find decoded indirect CALL instructions through a vtable's first slot.
// @category FDNY

import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.regex.Pattern;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

public class FindSlotZeroCalls extends GhidraScript {
    private static final Pattern SLOT_ZERO = Pattern.compile(
        "CALL\\s+dword ptr \\[(EAX|EBX|ECX|EDX|ESI|EDI|EBP|ESP)(?:\\s*\\+\\s*0x0)?\\]",
        Pattern.CASE_INSENSITIVE);

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) {
            throw new IllegalArgumentException("usage: FindSlotZeroCalls.java OUTPUT_FILE");
        }
        int matches = 0;
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            out.println("address\tfunction\tinstruction");
            InstructionIterator instructions = currentProgram.getListing().getInstructions(true);
            while (instructions.hasNext()) {
                monitor.checkCancelled();
                Instruction instruction = instructions.next();
                if (!instruction.getMnemonicString().toUpperCase().startsWith("CALL")) continue;
                String text = instruction.toString();
                if (!SLOT_ZERO.matcher(text).find()) continue;
                Function function = getFunctionContaining(instruction.getAddress());
                out.print(instruction.getAddress());
                out.print('\t');
                out.print(function == null ? "<unclaimed>" : function.getEntryPoint() + " " + function.getName());
                out.print('\t');
                out.println(text);
                matches++;
            }
        }
        println("Found " + matches + " slot-zero indirect calls");
    }
}
