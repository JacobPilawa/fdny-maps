import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class ExportHeldToolMatrixFunctions extends ghidra.app.script.GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) throw new IllegalArgumentException("Pass one output directory");
        Path outputDir = Paths.get(args[0]);
        Files.createDirectories(outputDir);
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        String[] targets = {"00497300", "00495D20", "00496020", "0049E770",
                            "00446EE0", "00447830"};
        for (String target : targets) {
            Address address = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(target);
            Function function = currentProgram.getFunctionManager().getFunctionAt(address);
            if (function == null) throw new IllegalStateException("No Ghidra function at " + target);
            DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
            if (!result.decompileCompleted() || result.getDecompiledFunction() == null)
                throw new IllegalStateException("Could not decompile " + target + ": " + result.getErrorMessage());
            String contents = "/* Hash-pinned retail EXE function " + target + "; names and types are provisional. */\n"
                + result.getDecompiledFunction().getC();
            Path output = outputDir.resolve(target.toLowerCase() + "_FUN_" + target.toLowerCase() + ".c");
            Files.writeString(output, contents, StandardCharsets.UTF_8);
            println("Exported " + function.getName() + " @ " + target + " to " + output);
        }
        decompiler.dispose();
    }
}
