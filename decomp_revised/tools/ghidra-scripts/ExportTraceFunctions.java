// Export hash-pinned Ghidra pseudocode for a bounded follow-up trace.
// Usage: -postScript ExportTraceFunctions.java <output.c> <input hash> <address> [address...]
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class ExportTraceFunctions extends ghidra.app.script.GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 3) throw new IllegalArgumentException("Pass output path, input hash, and function addresses");
        Path output = Paths.get(args[0]);
        StringBuilder contents = new StringBuilder();
        contents.append("/* Ghidra pseudocode export; names and types are provisional. */\n");
        contents.append("/* Program: ").append(currentProgram.getName()).append(" */\n\n");
        contents.append("/* Input SHA-256: ").append(args[1]).append(" */\n\n");
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        for (int i = 2; i < args.length; ++i) {
            String raw = args[i];
            String addressText = raw.startsWith("0x") ? raw : "0x" + raw;
            Address address = currentProgram.getAddressFactory().getDefaultAddressSpace()
                    .getAddress(addressText);
            Function function = currentProgram.getFunctionManager().getFunctionAt(address);
            if (function == null) function = currentProgram.getFunctionManager().getFunctionContaining(address);
            if (function == null) throw new IllegalStateException("No function at " + addressText);
            DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
            if (result == null || !result.decompileCompleted() || result.getDecompiledFunction() == null)
                throw new IllegalStateException("Decompilation failed at " + addressText + ": " +
                        (result == null ? "no result" : result.getErrorMessage()));
            contents.append("/* 0x").append(function.getEntryPoint()).append(" ")
                    .append(function.getName()).append(" */\n");
            contents.append(result.getDecompiledFunction().getC()).append("\n\n");
            println("Exported " + function.getName() + " @ " + addressText);
        }
        decompiler.dispose();
        Files.createDirectories(output.getParent());
        Files.writeString(output, contents.toString(), StandardCharsets.UTF_8);
        println("Wrote " + output);
    }
}
