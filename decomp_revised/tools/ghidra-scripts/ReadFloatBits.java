// Read binary32 and binary64 interpretations of addresses in the current program.
// Usage: -postScript ReadFloatBits.java 004dfa20 004dfa28 ...
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;

public class ReadFloatBits extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) throw new IllegalArgumentException("Pass one or more hexadecimal addresses");
        for (String value : args) {
            Address address = toAddr(value.startsWith("0x") ? value : "0x" + value);
            int bits32 = currentProgram.getMemory().getInt(address);
            long bits64 = currentProgram.getMemory().getLong(address);
            println(address + " f32=" + Float.intBitsToFloat(bits32) +
                    " bits32=0x" + Long.toHexString(bits32 & 0xffffffffL) +
                    " f64=" + Double.longBitsToDouble(bits64) +
                    " bits64=0x" + Long.toHexString(bits64));
        }
    }
}
