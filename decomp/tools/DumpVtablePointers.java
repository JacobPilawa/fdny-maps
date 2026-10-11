// Dump pointer words from selected original executable tables.
// @category FDNY

import java.io.FileWriter;
import java.io.PrintWriter;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class DumpVtablePointers extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException("usage: DumpVtablePointers.java OUTPUT_FILE [-count N] TABLE_ADDRESS...");
        }
        int firstTable = 1;
        int slotCount = 8;
        if (args.length >= 4 && "-count".equals(args[1])) {
            slotCount = Integer.parseInt(args[2]);
            firstTable = 3;
        }
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            out.println("table\tslot\tvalue\tfunction");
            for (int i = firstTable; i < args.length; i++) {
                Address table = toAddr(args[i].replace("0x", ""));
                for (int slot = 0; slot < slotCount; slot++) {
                    int value = getInt(table.add(slot * 4L));
                    String target = String.format("%08x", Integer.toUnsignedLong(value));
                    Function function = getFunctionAt(toAddr(target));
                    out.println(table + "\t" + slot + "\t" + target + "\t" +
                        (function == null ? "" : function.getName()));
                }
            }
        }
    }
}
