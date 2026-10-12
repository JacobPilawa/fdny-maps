// Export current Ghidra function boundaries to one CSV per program.
// Usage: -postScript ExportFunctionInventory.java <output-directory>

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.mem.MemoryBlock;

import java.io.BufferedWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class ExportFunctionInventory extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            throw new IllegalArgumentException("Pass an output directory as the first script argument.");
        }

        Path outDir = Paths.get(args[0]);
        Files.createDirectories(outDir);
        String safeName = currentProgram.getName().replaceAll("[^A-Za-z0-9._-]", "_");
        Path output = outDir.resolve(safeName + ".functions.csv");
        Path blocksOutput = outDir.resolve(safeName + ".memory-blocks.csv");

        try (BufferedWriter writer = Files.newBufferedWriter(output, StandardCharsets.UTF_8)) {
            writer.write("binary,entry_address,body_bytes,name,namespace,is_external,is_thunk,calling_convention,signature\n");
            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            long bodyFunctionCount = 0;
            while (functions.hasNext()) {
                monitor.checkCancelled();
                Function function = functions.next();
                writeFunction(writer, function);
                bodyFunctionCount++;
            }
            long externalFunctionCount = 0;
            FunctionIterator externalFunctions = currentProgram.getFunctionManager().getExternalFunctions();
            while (externalFunctions.hasNext()) {
                monitor.checkCancelled();
                writeFunction(writer, externalFunctions.next());
                externalFunctionCount++;
            }
            println("Wrote " + (bodyFunctionCount + externalFunctionCount) + " function records (" +
                bodyFunctionCount + " with bodies, " + externalFunctionCount + " external; manager count " +
                currentProgram.getFunctionManager().getFunctionCount() + ") to " + output);
        }

        try (BufferedWriter writer = Files.newBufferedWriter(blocksOutput, StandardCharsets.UTF_8)) {
            writer.write("binary,name,start_address,end_address,size_bytes,read,write,execute,initialized\n");
            for (MemoryBlock block : currentProgram.getMemory().getBlocks()) {
                monitor.checkCancelled();
                writer.write(csv(currentProgram.getName()));
                writer.write(",");
                writer.write(csv(block.getName()));
                writer.write(",");
                writer.write(csv(block.getStart().toString()));
                writer.write(",");
                writer.write(csv(block.getEnd().toString()));
                writer.write(",");
                writer.write(Long.toString(block.getSize()));
                writer.write(",");
                writer.write(Boolean.toString(block.isRead()));
                writer.write(",");
                writer.write(Boolean.toString(block.isWrite()));
                writer.write(",");
                writer.write(Boolean.toString(block.isExecute()));
                writer.write(",");
                writer.write(Boolean.toString(block.isInitialized()));
                writer.newLine();
            }
            println("Wrote memory blocks to " + blocksOutput);
        }
    }

    private void writeFunction(BufferedWriter writer, Function function) throws Exception {
        writer.write(csv(currentProgram.getName()));
        writer.write(",");
        writer.write(csv(function.getEntryPoint().toString()));
        writer.write(",");
        writer.write(Long.toString(function.getBody().getNumAddresses()));
        writer.write(",");
        writer.write(csv(function.getName()));
        writer.write(",");
        writer.write(csv(function.getParentNamespace().getName(true)));
        writer.write(",");
        writer.write(Boolean.toString(function.isExternal()));
        writer.write(",");
        writer.write(Boolean.toString(function.isThunk()));
        writer.write(",");
        writer.write(csv(function.getCallingConventionName()));
        writer.write(",");
        writer.write(csv(function.getSignature().toString()));
        writer.newLine();
    }

    private static String csv(String value) {
        if (value == null) {
            return "\"\"";
        }
        return "\"" + value.replace("\"", "\"\"") + "\"";
    }
}
