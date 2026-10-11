// Export code and data references to selected original addresses.
// @category FDNY

import java.io.File;
import java.io.PrintWriter;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;

public class FindReferences extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException("usage: FindReferences.java OUTPUT_FILE ADDRESS...");
        }
        try (PrintWriter out = new PrintWriter(new File(args[0]))) {
            out.println("target\tfrom\ttype\tfunction");
            for (int i = 1; i < args.length; i++) {
                Address target = toAddr(args[i].replace("0x", ""));
                for (Reference reference : getReferencesTo(target)) {
                    Address from = reference.getFromAddress();
                    Function function = getFunctionContaining(from);
                    out.print(target + "\t" + from + "\t" + reference.getReferenceType() + "\t");
                    out.println(function == null ? "<data>" : function.getEntryPoint() + " " + function.getName());
                }
            }
        }
    }
}
