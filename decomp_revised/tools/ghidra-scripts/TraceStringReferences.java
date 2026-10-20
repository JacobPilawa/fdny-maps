// Find defined strings and list their code/data xrefs with containing functions.
// Usage: -postScript TraceStringReferences.java <substring> [substring...]
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import java.util.Locale;

public class TraceStringReferences extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) throw new IllegalArgumentException("Pass one or more string substrings");
        DataIterator iterator = currentProgram.getListing().getDefinedData(true);
        while (iterator.hasNext()) {
            Data data = iterator.next();
            String representation = data.getDefaultValueRepresentation();
            if (representation == null) continue;
            for (String argument : args) {
                if (!representation.toLowerCase(Locale.ROOT).contains(argument.toLowerCase(Locale.ROOT))) continue;
                println("STRING " + data.getAddress() + " :: " + representation);
                ReferenceIterator references = currentProgram.getReferenceManager().getReferencesTo(data.getAddress());
                while (references.hasNext()) {
                    Reference reference = references.next();
                    Function owner = currentProgram.getFunctionManager().getFunctionContaining(reference.getFromAddress());
                    println("  XREF " + reference.getFromAddress() + " " + reference.getReferenceType() +
                            " owner=" + (owner == null ? "<none>" : owner.getEntryPoint() + " " + owner.getName()));
                }
            }
        }
    }
}
