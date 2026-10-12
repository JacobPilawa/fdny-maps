// Export a lightweight address inventory from an already analyzed Ghidra
// program. This does not decompile functions or mutate the program database.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class ExportFunctionInventory extends GhidraScript {
    private String quote(String value) {
        StringBuilder out = new StringBuilder("\"");
        for (int i = 0; i < value.length(); i++) {
            char c = value.charAt(i);
            if (c == '"' || c == '\\') out.append('\\').append(c);
            else if (c == '\n') out.append("\\n");
            else if (c == '\r') out.append("\\r");
            else if (c == '\t') out.append("\\t");
            else if (c < 0x20) out.append(String.format("\\u%04x", (int)c));
            else out.append(c);
        }
        return out.append('"').toString();
    }

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1 || args.length > 3)
            throw new IllegalArgumentException("Usage: ExportFunctionInventory.java <output.json> [executable-sha256] [ghidra-version]");

        List<Function> functions = new ArrayList<>();
        FunctionIterator iterator = currentProgram.getFunctionManager().getFunctions(true);
        while (iterator.hasNext()) functions.add(iterator.next());
        functions.sort(Comparator.comparing(Function::getEntryPoint));

        File output = new File(args[0]);
        File parent = output.getParentFile();
        if (parent != null) parent.mkdirs();
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(output))) {
            writer.write("{\n  \"program\": " + quote(currentProgram.getName()) + ",\n");
            writer.write("  \"language\": " + quote(currentProgram.getLanguageID().toString()) + ",\n");
            writer.write("  \"compilerSpec\": " + quote(currentProgram.getCompilerSpec().getCompilerSpecID().toString()) + ",\n");
            writer.write("  \"executableSha256\": " + quote(args.length > 1 ? args[1] : "unspecified") + ",\n");
            writer.write("  \"ghidraVersion\": " + quote(args.length > 2 ? args[2] : "unspecified") + ",\n");
            writer.write("  \"generator\": \"ExportFunctionInventory.java; -noanalysis -readOnly\",\n");
            writer.write("  \"functionCount\": " + functions.size() + ",\n");
            writer.write("  \"scope\": \"Functions currently defined in this Ghidra database; inventory only, not proof that every code routine was discovered or classified.\",\n");
            writer.write("  \"functions\": [\n");
            for (int i = 0; i < functions.size(); i++) {
                Function function = functions.get(i);
                String address = String.format("%08X", function.getEntryPoint().getOffset());
                long bodyBytes = function.getBody().getNumAddresses();
                writer.write("    {\"address\": " + quote(address)
                    + ", \"name\": " + quote(function.getName())
                    + ", \"bodyBytes\": " + bodyBytes
                    + ", \"thunk\": " + function.isThunk()
                    + ", \"external\": " + function.isExternal()
                    + ", \"status\": \"unclassified\"}");
                writer.write(i + 1 == functions.size() ? "\n" : ",\n");
            }
            writer.write("  ]\n}\n");
        }
        println("Wrote " + functions.size() + " function entries to " + output.getAbsolutePath());
    }
}
