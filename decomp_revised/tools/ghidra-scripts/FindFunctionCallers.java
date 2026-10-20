// Print direct code references to selected function entries.
// Usage: -postScript FindFunctionCallers.java 0048D330 [address ...]
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;

public class FindFunctionCallers extends GhidraScript {
    @Override
    public void run() throws Exception {
        for (String raw : getScriptArgs()) {
            String text = raw.startsWith("0x") ? raw : "0x" + raw;
            Address target = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(text);
            Function function = currentProgram.getFunctionManager().getFunctionAt(target);
            if (function == null) throw new IllegalStateException("No function at " + target);
            println("TARGET " + function.getEntryPoint() + " " + function.getName());
            ReferenceIterator refs = currentProgram.getReferenceManager().getReferencesTo(target);
            while (refs.hasNext()) {
                Reference ref = refs.next();
                Function caller = currentProgram.getFunctionManager().getFunctionContaining(ref.getFromAddress());
                Instruction instruction = currentProgram.getListing().getInstructionAt(ref.getFromAddress());
                println("  " + ref.getFromAddress() + " " +
                        (caller == null ? "<no function>" : caller.getEntryPoint() + " " + caller.getName()) +
                        " " + ref.getReferenceType() + " :: " + (instruction == null ? "<no instruction>" : instruction.toString()));
            }
        }
    }
}
