// Preserve scalar words from the read-only original executable.
// @category FDNY

import java.io.FileWriter;
import java.io.PrintWriter;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;

public class DumpScalarWords extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException("usage: DumpScalarWords.java OUTPUT_FILE ADDRESS...");
        }
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            out.println("address\tword0_hex\tword1_hex");
            for (int i = 1; i < args.length; i++) {
                Address address = toAddr(args[i].replace("0x", ""));
                out.printf("%s\t%08x\t%08x%n", address, getInt(address), getInt(address.add(4)));
            }
        }
    }
}
