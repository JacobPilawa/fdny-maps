// Find decoded instructions whose printed operands contain selected text.
// @category FDNY

import java.io.FileWriter;
import java.io.PrintWriter;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

public class FindInstructionText extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException("usage: FindInstructionText.java OUTPUT_FILE TEXT...");
        }
        int matches = 0;
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            out.println("address\tfunction\tinstruction");
            InstructionIterator instructions = currentProgram.getListing().getInstructions(true);
            while (instructions.hasNext()) {
                monitor.checkCancelled();
                Instruction instruction = instructions.next();
                String text = instruction.toString();
                boolean found = false;
                for (int i = 1; i < args.length; i++) {
                    found |= text.toLowerCase().contains(args[i].toLowerCase());
                }
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
        println("Found " + matches + " matching instructions");
    }
}
