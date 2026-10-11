import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

/** Print Ghidra C decompilation for explicitly requested functions. */
public class DecompileFunctions extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] requested = getScriptArgs();
        if (requested.length == 0) {
            println("Usage: DecompileFunctions.java <entry-address> [entry-address ...]");
            return;
        }

        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        try {
            for (String text : requested) {
                String normalized = text.startsWith("0x") || text.startsWith("0X")
                    ? text.substring(2) : text;
                Address address = toAddr(normalized);
                Function function = getFunctionAt(address);
                if (function == null) {
                    println("NO FUNCTION at " + address);
                    continue;
                }
                DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
                println("FUNCTION " + function.getEntryPoint() + " " + function.getName());
                if (result.decompileCompleted() && result.getDecompiledFunction() != null) {
                    println(result.getDecompiledFunction().getC());
                } else {
                    println("DECOMPILE FAILED: " + result.getErrorMessage());
                }
            }
        } finally {
            decompiler.dispose();
        }
    }
}
