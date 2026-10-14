import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;

public class AuditHeldItemFlow extends ghidra.app.script.GhidraScript {
    private void dump(DecompInterface decompiler, Address address) throws Exception {
        Function function = currentProgram.getFunctionManager().getFunctionAt(address);
        if (function == null) function = currentProgram.getFunctionManager().getFunctionContaining(address);
        println("===== TARGET " + address + " =====");
        if (function == null) {
            println("No function body");
            return;
        }
        DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
        println("FUNCTION " + function.getName() + " @ " + function.getEntryPoint());
        if (result.getErrorMessage() != null && !result.getErrorMessage().isEmpty())
            println("DECOMPILER ERROR: " + result.getErrorMessage());
        println(result.getDecompiledFunction() == null ? "<no decompiled body>" :
                 result.getDecompiledFunction().getC());
        println("CALLERS:");
        for (Reference reference : currentProgram.getReferenceManager().getReferencesTo(function.getEntryPoint())) {
            Function caller = currentProgram.getFunctionManager().getFunctionContaining(reference.getFromAddress());
            println("  " + reference.getFromAddress() + " from " +
                    (caller == null ? "<no caller function>" : caller.getName() + " @ " + caller.getEntryPoint()));
        }
    }

    @Override
    public void run() throws Exception {
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        String[] targets = {"0045F660", "0045F7E0", "00460250", "00460A50",
                            "00461630", "00461690", "00461810", "00461820",
                            "00461830", "00461F80", "0045F970", "00461260",
                            "00461270"};
        for (String target : targets) {
            Address address = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(target);
            dump(decompiler, address);
        }
        decompiler.dispose();
    }
}
