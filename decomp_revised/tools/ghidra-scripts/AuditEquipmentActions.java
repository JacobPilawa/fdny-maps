import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;

/** Dump hash-pinned retail equipment action functions and their direct callers. */
public class AuditEquipmentActions extends ghidra.app.script.GhidraScript {
    private void dump(DecompInterface decompiler, String target) throws Exception {
        Address address = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(target);
        Function function = currentProgram.getFunctionManager().getFunctionAt(address);
        if (function == null) function = currentProgram.getFunctionManager().getFunctionContaining(address);
        if (function == null) {
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
        println("DIRECT REFERENCES:");
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
        String[] constants = {
            "004dd808", "004e16c4", "004e27e8", "004e27e0",
            "004e1b30", "004e1840", "004e27d8", "004de6f8",
            "004e2924", "004df224", "004dfa28", "004dfa10",
            "004dfa30", "004dc50c"
        };
        for (String target : constants) {
            Address address = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(target);
            byte[] bytes = new byte[24];
            Address start = address.subtract(8);
            currentProgram.getMemory().getBytes(start, bytes);
            StringBuilder hex = new StringBuilder();
            for (byte value : bytes) hex.append(String.format("%02x", value & 0xff));
            println("CONSTANT_BYTES " + target + " nearby=" + hex);
        }
        String[] targets = {
            "0044F340", "0045F660", "0045FEC0", "0045FB90", "00460250", "00460920", "00460AD0", "00460AE0", "00460AF0", "00460B10", "00460B90",
            "004612B0", "00461260", "00461270", "004614E0", "00461590",
            "00461630", "00461690", "00461810", "00461820", "00461830",
            "00461F60", "00461F80", "0042D280", "0047DB10", "0047FE70", "0047FF30", "0047F1C0", "0047F1B0",
            "0047F850", "0047F500", "0047D370", "0047D6D0", "0047D8A0", "0047F960", "00480260", "00482430", "00481A50", "00481A60",
            "00497F70", "00492450", "004926E0",
            "0049F260", "0049F340", "004A0BE0", "004A0B10", "004A11A0", "004A11D0", "004A12F0"
        };
        for (String target : targets) dump(decompiler, target);
        decompiler.dispose();
    }
}
