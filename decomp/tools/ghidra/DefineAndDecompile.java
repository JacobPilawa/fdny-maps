import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

/** Create a missing function at a verified entry and print its Ghidra decompilation.
 *  Use in a temporary analysis project; defining a function changes the project.
 */
public class DefineAndDecompile extends GhidraScript {
    @Override
    public void run() throws Exception {
        if (getScriptArgs().length == 0) {
            println("Usage: DefineAndDecompile.java <entry-address>");
            return;
        }

        String text = getScriptArgs()[0];
        String normalized = text.startsWith("0x") || text.startsWith("0X")
            ? text.substring(2) : text;
        Address entry = toAddr(normalized);
        Function function = getFunctionAt(entry);
        if (function == null) {
            function = createFunction(entry, null);
        }
        if (function == null) {
            println("Could not define function at " + entry);
            return;
        }

        println("FUNCTION " + function.getEntryPoint() + " " + function.getName());
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        try {
            DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
            if (result.decompileCompleted() && result.getDecompiledFunction() != null) {
                println(result.getDecompiledFunction().getC());
            } else {
                println("DECOMPILE FAILED: " + result.getErrorMessage());
            }
        } finally {
            decompiler.dispose();
        }
    }
}
