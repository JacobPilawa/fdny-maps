// Show the defined data item, raw bytes, and incoming references for data addresses.
// Usage: -postScript InspectDataAtAddress.java 00502d40 [00502d78 ...]
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;

public class InspectDataAtAddress extends GhidraScript {
    @Override public void run() throws Exception {
        for (String raw : getScriptArgs()) {
            Address a = toAddr(raw.startsWith("0x") ? raw : "0x" + raw);
            println("--- DATA " + a + " ---");
            Data item = currentProgram.getListing().getDataAt(a);
            if (item == null) item = currentProgram.getListing().getDataContaining(a);
            println("ITEM " + (item == null ? "<undefined>" : item.getAddress() + " " + item.getDataType() + " " + item.getDefaultValueRepresentation()));
            byte[] bytes = new byte[0x40];
            try {
                currentProgram.getMemory().getBytes(a, bytes);
                StringBuilder line = new StringBuilder();
                for (int i = 0; i < bytes.length; i++) {
                    if (i % 16 == 0) {
                        if (line.length() > 0) println(line.toString());
                        line = new StringBuilder(a.add(i).toString() + ":");
                    }
                    line.append(String.format(" %02X", bytes[i] & 0xff));
                }
                if (line.length() > 0) println(line.toString());
            } catch (Exception e) { println("BYTE_ERROR " + e.getMessage()); }
            ReferenceIterator refs = currentProgram.getReferenceManager().getReferencesTo(a);
            while (refs.hasNext()) {
                Reference ref = refs.next();
                println("XREF " + ref.getFromAddress() + " " + ref.getReferenceType());
            }
        }
    }
}
