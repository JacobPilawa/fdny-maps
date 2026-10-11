import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;

/** Print incoming and outgoing references for one or more addresses. */
public class AddressXrefs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] requested = getScriptArgs();
        if (requested.length == 0) {
            println("Usage: AddressXrefs.java <address> [address ...]");
            return;
        }

        for (String text : requested) {
            String normalized = text.startsWith("0x") || text.startsWith("0X")
                ? text.substring(2) : text;
            Address address = toAddr(normalized);
            println("ADDRESS " + address);

            println("  INCOMING");
            for (Reference reference : currentProgram.getReferenceManager().getReferencesTo(address)) {
                Function function = getFunctionContaining(reference.getFromAddress());
                println("    " + reference.getFromAddress() + " " + reference.getReferenceType()
                    + " function=" + (function == null ? "<none>"
                        : function.getEntryPoint() + " " + function.getName()));
            }

            println("  OUTGOING");
            for (Reference reference : currentProgram.getReferenceManager().getReferencesFrom(address)) {
                println("    " + reference.getToAddress() + " " + reference.getReferenceType());
            }
        }
    }
}
