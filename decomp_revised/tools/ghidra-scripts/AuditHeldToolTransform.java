import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;

public class AuditHeldToolTransform extends ghidra.app.script.GhidraScript {
    private void dump(DecompInterface decompiler, String target) throws Exception {
        Address address = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(target);
        Function function = currentProgram.getFunctionManager().getFunctionAt(address);
        if (function == null) function = currentProgram.getFunctionManager().getFunctionContaining(address);
        if (function == null && (target.equals("00497F70") || target.equals("00461830"))) {
            disassemble(address);
            function = createFunction(address, "Recovered_" + target);
        }
        println("===== TARGET " + target + " =====");
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
        String[] targets = {"0042D2D0", "0042D280", "0044F340", "0045F660",
                            "00460250", "00461830", "0047D370", "0047F500",
                            "004922A0",
                            "0047D2B0", "0047D300", "0047DB10", "0047DBD0",
                            "0047F850", "00497E90",
                            "00497F70", "00494500", "004924B0", "00496020",
                            "0049E770", "00447830", "00446EE0", "00497300",
                            "00495D20",
                            "0042D0A0", "004977B0",
                            "00492110", "00492450", "004926E0"};
        for (String target : targets) dump(decompiler, target);
        decompiler.dispose();
    }
}
