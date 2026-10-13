// Export selected Genesis.dll motion parser/extent functions for source-backed
// format recovery. Run with analyzeHeadless -postScript and an output path arg.
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.PrintWriter;

public class ExportMotionFunctions extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            printerr("Usage: ExportMotionFunctions.java <output-file>");
            return;
        }

        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        ConsoleTaskMonitor monitor = new ConsoleTaskMonitor();
        Listing listing = currentProgram.getListing();
        try (PrintWriter out = new PrintWriter(new File(args[0]))) {
            String[] names = {
                "geMotion_CreateFromFile", "gePath_CreateFromFile",
                "geMotion_GetTimeExtents", "gePath_GetTimeExtents",
                "geMotion_GetPath", "gePath_GetKeyframeCount"
            };
            for (String name : names) {
                Function function = findFunctionByName(name);
                if (function == null) {
                    out.println("===== MISSING: " + name + " =====");
                    continue;
                }
                writeFunction(out, function, decompiler, listing, monitor);
            }
            String[] helperAddresses = {
                "1000f180", "1000eab0", "10005a00", "10015fb0",
                "10014720", "100163d0", "10006080"
            };
            for (String address : helperAddresses) {
                Function function = currentProgram.getFunctionManager().getFunctionAt(toAddr(address));
                if (function == null) {
                    out.println("===== MISSING helper @ " + address + " =====");
                    continue;
                }
                writeFunction(out, function, decompiler, listing, monitor);
            }
        } finally {
            decompiler.dispose();
        }
    }

    private Function findFunctionByName(String name) {
        FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
        while (functions.hasNext()) {
            Function function = functions.next();
            if (function.getName().equals(name) || function.getName().startsWith(name + "@")) {
                return function;
            }
        }
        return null;
    }

    private void writeFunction(PrintWriter out, Function function, DecompInterface decompiler,
                               Listing listing, ConsoleTaskMonitor monitor) {
        out.println("===== " + function.getName() + " @ " + function.getEntryPoint() + " =====");
        DecompileResults result = decompiler.decompileFunction(function, 120, monitor);
        if (result != null && result.decompileCompleted() && result.getDecompiledFunction() != null) {
            out.println(result.getDecompiledFunction().getC());
        } else {
            out.println("DECOMPILER: " + (result == null ? "no result" : result.getErrorMessage()));
        }
        out.println("--- DISASSEMBLY ---");
        InstructionIterator instructions = listing.getInstructions(function.getBody(), true);
        while (instructions.hasNext()) {
            Instruction instruction = instructions.next();
            out.printf("%s  %s%n", instruction.getAddress(), instruction);
        }
        out.println();
    }
}
