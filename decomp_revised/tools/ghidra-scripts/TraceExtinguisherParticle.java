import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;

/** Dump particle-pool vtables and implementation methods for extinguisher spray. */
public class TraceExtinguisherParticle extends ghidra.app.script.GhidraScript {
    private void dumpFunction(DecompInterface decompiler, Address address) throws Exception {
        Function function = currentProgram.getFunctionManager().getFunctionAt(address);
        if (function == null) function = currentProgram.getFunctionManager().getFunctionContaining(address);
        println("FUNCTION " + address + " => " +
                (function == null ? "<no function>" : function.getName() + " @ " + function.getEntryPoint()));
        if (function == null) return;
        DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
        println(result.getDecompiledFunction() == null ? "<no decompiled body>" :
                result.getDecompiledFunction().getC());
    }

    @Override
    public void run() throws Exception {
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        String[] vtables = {"004e3548", "004e3520", "004e354c"};
        for (String table : vtables) {
            Address base = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(table);
            println("===== VTABLE " + table + " =====");
            for (int slot = 0; slot < 24; slot++) {
                Address entry = base.add(slot * 4L);
                int pointer = currentProgram.getMemory().getInt(entry);
                Address target = currentProgram.getAddressFactory().getDefaultAddressSpace()
                        .getAddress(Integer.toUnsignedString(pointer, 16));
                Function function = currentProgram.getFunctionManager().getFunctionAt(target);
                println("slot " + slot + " entry " + entry + " -> " + target + " " +
                        (function == null ? "<no function>" : function.getName()));
            }
        }
        String[] targets = {"00474EC0", "00480560", "0047FEA0", "0047F960", "0047F500",
                            "00480540", "0047D330", "0047D370", "0047D420", "0047D560",
                            "0047D6D0", "0047D820", "0047D860", "0047D960", "0047F2A0",
                            "0047F5A0", "0047F1A0", "0047F1B0", "0047F1C0", "0047DB10",
                            "0047DBA0", "00493D00", "00480350", "00463700", "00463EF0",
                            "0044D080", "004634F0", "00461690", "00461830", "00461F80",
                            "0047F850", "0047FE70", "0047FF30", "0047D330", "0047D370",
                            "0047D660", "0047D6D0", "0047D8A0", "0047D960", "0047D560",
                            "0047D3E0", "0047D400", "00493650", "00493D60", "0047D690"};
        for (String target : targets) {
            Address address = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(target);
            println("REFERENCES TO " + target + ":");
            for (Reference reference : currentProgram.getReferenceManager().getReferencesTo(address)) {
                Function caller = currentProgram.getFunctionManager().getFunctionContaining(reference.getFromAddress());
                println("  " + reference.getFromAddress() + " from " +
                        (caller == null ? "<no caller function>" : caller.getName() + " @ " + caller.getEntryPoint()));
            }
            dumpFunction(decompiler, address);
        }
        decompiler.dispose();
    }
}
