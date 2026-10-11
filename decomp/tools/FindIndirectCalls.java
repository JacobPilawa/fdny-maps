// Find virtual/indirect calls through selected object-vtable byte offsets.
// @category FDNY

import java.io.File;
import java.io.PrintWriter;
import java.util.regex.Pattern;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

public class FindIndirectCalls extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException("usage: FindIndirectCalls.java OUTPUT_FILE OFFSET...");
        }
        Pattern[] patterns = new Pattern[args.length - 1];
        for (int i = 1; i < args.length; i++) {
            String offset = args[i].replace("0x", "");
            patterns[i - 1] = Pattern.compile("\\[[^\\]]+\\+\\s*0x" + offset + "\\]", Pattern.CASE_INSENSITIVE);
        }
        int matches = 0;
        try (PrintWriter out = new PrintWriter(new File(args[0]))) {
            out.println("address\tfunction\tinstruction");
            InstructionIterator instructions = currentProgram.getListing().getInstructions(true);
            while (instructions.hasNext()) {
                monitor.checkCancelled();
                Instruction instruction = instructions.next();
                if (!instruction.getMnemonicString().toUpperCase().startsWith("CALL")) continue;
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
        println("Found " + matches + " indirect calls");
    }
}
