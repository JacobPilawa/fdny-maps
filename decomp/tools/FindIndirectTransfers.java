// Find decoded indirect CALL/JMP instructions through selected object-vtable offsets.
// @category FDNY

import java.io.File;
import java.io.PrintWriter;
import java.util.regex.Pattern;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

public class FindIndirectTransfers extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException("usage: FindIndirectTransfers.java OUTPUT_FILE OFFSET...");
        }
        Pattern[] patterns = new Pattern[args.length - 1];
        for (int i = 1; i < args.length; i++) {
            String offset = args[i].replace("0x", "");
            if ("0".equals(offset)) {
                // Zero-offset virtual dispatch may stay as [EAX], or the
                // function pointer may be loaded first and called as CALL EAX.
                patterns[i - 1] = Pattern.compile(
                    "(?:\\[(?:EAX|EBX|ECX|EDX|ESI|EDI|EBP|ESP)(?:\\s*\\+\\s*0x0)?\\]"
                    + "|\\b(?:CALL|JMP)\\s+(?:(?:dword ptr)\\s*)?(?:EAX|EBX|ECX|EDX|ESI|EDI|EBP|ESP)\\b)",
                    Pattern.CASE_INSENSITIVE);
            } else {
                patterns[i - 1] = Pattern.compile("\\[[^\\]]+\\+\\s*0x" + offset + "\\]",
                    Pattern.CASE_INSENSITIVE);
            }
        }
        int matches = 0;
        try (PrintWriter out = new PrintWriter(new File(args[0]))) {
            out.println("address\tfunction\tinstruction");
            InstructionIterator instructions = currentProgram.getListing().getInstructions(true);
            while (instructions.hasNext()) {
                monitor.checkCancelled();
                Instruction instruction = instructions.next();
                String mnemonic = instruction.getMnemonicString().toUpperCase();
                if (!mnemonic.startsWith("CALL") && !mnemonic.startsWith("JMP")) continue;
                String text = instruction.toString();
                boolean found = false;
                for (Pattern pattern : patterns) found |= pattern.matcher(text).find();
                if (!found) continue;
                Function function = getFunctionContaining(instruction.getAddress());
                out.print(instruction.getAddress());
                out.print('\t');
                out.print(function == null ? "<unclaimed>" : function.getEntryPoint() + " " + function.getName());
                out.print('\t');
                out.println(text);
                matches++;
            }
        }
        println("Found " + matches + " indirect CALL/JMP transfers");
    }
}
