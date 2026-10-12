import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

/** Export read-only Ghidra C for an explicit set of original function entries. */
public class ExportFunctionC extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("Usage: ExportFunctionC.java <output-directory> [--define-missing] <entry-address> [entry-address ...]");
            return;
        }

        Path output = Path.of(args[0]);
        Files.createDirectories(output);
        boolean defineMissing = args.length > 1 && args[1].equals("--define-missing");
        int firstAddress = defineMissing ? 2 : 1;
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        int exported = 0;
        int defined = 0;
        try {
            for (int index = firstAddress; index < args.length; index++) {
                String text = args[index];
                String normalized = text.startsWith("0x") || text.startsWith("0X")
                    ? text.substring(2) : text;
                Address address = toAddr(normalized);
                Function function = getFunctionAt(address);
                boolean newlyDefined = false;
                if (function == null) {
                    function = getFunctionContaining(address);
                }
                if (function == null && defineMissing) {
                    function = createFunction(address, null);
                    newlyDefined = function != null;
                    if (newlyDefined) defined++;
                }
                String fileName = normalized.toUpperCase() + ".c";
                Path file = output.resolve(fileName);
                if (function == null) {
                    Files.writeString(file,
                        "/* Ghidra has no function boundary containing " + address + ". */\n",
                        StandardCharsets.UTF_8);
                    println("NO FUNCTION " + address);
                    continue;
                }

                DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
                StringBuilder source = new StringBuilder();
                source.append("/* Read-only Ghidra C decompilation.\n")
                    .append(" * Program: ").append(currentProgram.getName()).append("\n")
                    .append(" * Requested address: ").append(address).append("\n")
                    .append(" * Match: ").append(newlyDefined ? "scratch-defined function boundary" :
                        (function.getEntryPoint().equals(address) ? "function entry" : "inside function"))
                    .append("\n")
                    .append(" * Entry: ").append(function.getEntryPoint()).append(" ")
                    .append(function.getName()).append("\n")
                    .append(" * Generated from the preserved Ghidra database; see tools/ghidra/README.md.\n")
                    .append(" */\n\n");
                if (result.decompileCompleted() && result.getDecompiledFunction() != null) {
                    source.append(result.getDecompiledFunction().getC()).append('\n');
                    exported++;
                    println("EXPORTED " + address + " " + function.getEntryPoint() + " " + fileName);
                } else {
                    source.append("/* DECOMPILE FAILED: ")
                        .append(result.getErrorMessage().replace("*/", "* /"))
                        .append(" */\n");
                    println("DECOMPILE FAILED " + function.getEntryPoint() + " " + result.getErrorMessage());
                }
                Files.writeString(file, source, StandardCharsets.UTF_8);
            }
        } finally {
            decompiler.dispose();
        }
        println("Ghidra C exports: " + exported + " of " + (args.length - firstAddress)
            + "; scratch-defined boundaries: " + defined);
    }
}
