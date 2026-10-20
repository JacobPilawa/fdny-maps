// Scan the current program for x86 instructions that mention selected offsets.
// Usage: -postScript SearchPlayerOffsets.java OUTPUT_PATH 3420 3438 ...
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Locale;
import java.util.regex.Pattern;

public class SearchPlayerOffsets extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) throw new IllegalArgumentException("Pass an output path and field offsets");
        Path output = Paths.get(args[0]);
        ArrayList<Pattern> patterns = new ArrayList<>();
        for (int i = 1; i < args.length; i++) {
            String value = args[i].toLowerCase(Locale.ROOT).replaceFirst("^0x", "");
            patterns.add(Pattern.compile("(?i)(?:\\+\\s*0x0*" + value + "\\b|\\b0x0*" + value + "\\b)"));
        }
        StringBuilder lines = new StringBuilder();
        InstructionIterator iterator = currentProgram.getListing().getInstructions(true);
        while (iterator.hasNext()) {
            Instruction instruction = iterator.next();
            String representation = instruction.toString();
            boolean match = false;
            for (Pattern pattern : patterns) {
                if (pattern.matcher(representation).find()) { match = true; break; }
            }
            if (!match) continue;
            Function function = currentProgram.getFunctionManager().getFunctionContaining(instruction.getAddress());
            lines.append(instruction.getAddress()).append(" ")
                 .append(function == null ? "<no function>" : function.getEntryPoint() + " " + function.getName())
                 .append(" :: ").append(representation).append('\n');
        }
        Files.writeString(output, lines.toString(), StandardCharsets.UTF_8);
        println("Wrote " + output + " with " + lines.toString().lines().count() + " matches");
    }
}
