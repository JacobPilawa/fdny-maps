// Dump function pointers from one or more vtable addresses in the current program.
// Usage: -postScript DumpVtable.java 004e0588 004dfd14
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class DumpVtable extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) throw new IllegalArgumentException("Pass one or more vtable addresses");
        for (String argument : args) {
            Address base = toAddr(argument.startsWith("0x") ? argument : "0x" + argument);
            println("--- VTABLE " + base + " ---");
            for (int slot = 0; slot < 24; slot++) {
                int raw = currentProgram.getMemory().getInt(base.add(slot * 4L));
                long pointer = Integer.toUnsignedLong(raw);
                Address target = toAddr("0x" + Long.toHexString(pointer));
                Function function = getFunctionAt(target);
                println(String.format("%02d  %08x  %s", slot, pointer,
                    function == null ? "<no function label>" : function.getName()));
            }
        }
    }
}
