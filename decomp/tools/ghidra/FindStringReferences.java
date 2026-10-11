import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.data.StringDataInstance;

/** Find defined string data containing requested text and list its references. */
public class FindStringReferences extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] requested = getScriptArgs();
        if (requested.length == 0) {
            println("Usage: FindStringReferences.java <substring> [substring ...]");
            return;
        }

        DataIterator dataIterator = currentProgram.getListing().getDefinedData(true);
        while (dataIterator.hasNext() && !monitor.isCancelled()) {
            Data data = dataIterator.next();
            if (!data.hasStringValue()) continue;

            String value = StringDataInstance.getStringDataInstance(data).getStringValue();
            if (value == null) continue;
            for (String query : requested) {
                if (!value.toLowerCase().contains(query.toLowerCase())) continue;
                println("STRING " + data.getAddress() + " query=" + query + " value=" + value);
                for (int offset = 0; offset < data.getLength(); offset++) {
                    Address target = data.getAddress().add(offset);
                    for (Reference reference : currentProgram.getReferenceManager()
                            .getReferencesTo(target)) {
                        Function function = getFunctionContaining(reference.getFromAddress());
                        println("  REF target=" + target + " offset=" + offset + " from="
                            + reference.getFromAddress() + " " + reference.getReferenceType()
                            + " function=" + (function == null ? "<none>"
                                : function.getEntryPoint() + " " + function.getName()));
                    }
                }
            }
        }
    }
}
