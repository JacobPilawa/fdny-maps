import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;

/** Print a defined data item and raw integer/float interpretations at explicit addresses. */
public class DataValue extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] requested = getScriptArgs();
        if (requested.length == 0) {
            println("Usage: DataValue.java <address> [address ...]");
            return;
        }

        for (String text : requested) {
            String normalized = text.startsWith("0x") || text.startsWith("0X")
                ? text.substring(2) : text;
            Address address = toAddr(normalized);
            println("ADDRESS " + address);
            Data data = currentProgram.getListing().getDataAt(address);
            println("  DEFINED_DATA " + (data == null ? "<none>" : data));
            try {
                int bits = currentProgram.getMemory().getInt(address);
                println(String.format("  INT32 0x%08X (%d)", bits, bits));
                println(String.format("  FLOAT32 %.9g", Float.intBitsToFloat(bits)));
                println(String.format("  BYTES %02X %02X %02X %02X",
                    bits & 0xff, (bits >>> 8) & 0xff, (bits >>> 16) & 0xff,
                    (bits >>> 24) & 0xff));
            } catch (Exception e) {
                println("  READ_FAILED " + e.getMessage());
            }
            try {
                long bits = currentProgram.getMemory().getLong(address);
                println(String.format("  INT64 0x%016X (%d)", bits, bits));
                println(String.format("  FLOAT64 %.17g", Double.longBitsToDouble(bits)));
            } catch (Exception e) {
                println("  READ64_FAILED " + e.getMessage());
            }
        }
    }
}
