// Export decoded instructions around addresses without inventing function boundaries.
// @category FDNY

import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;

public class ExportInstructionWindows extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 3) {
            throw new IllegalArgumentException(
                "usage: ExportInstructionWindows.java OUTPUT_FILE RADIUS ADDRESS...");
        }
        int radius = Integer.parseInt(args[1]);
        if (radius < 0) throw new IllegalArgumentException("radius must be nonnegative");
        Listing listing = currentProgram.getListing();
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            for (int n = 2; n < args.length; n++) {
                Address address = toAddr(args[n].replace("0x", ""));
                if (address == null) {
                    out.println("SKIP " + args[n] + " unresolved-address");
                    continue;
                }
                Instruction center = listing.getInstructionAt(address);
                if (center == null) {
                    out.println("SKIP " + address + " no-instruction");
                    continue;
                }

                ArrayList<Instruction> before = new ArrayList<>();
                Instruction cursor = center;
                for (int i = 0; i < radius; i++) {
                    cursor = listing.getInstructionBefore(cursor.getAddress());
                    if (cursor == null) break;
                    before.add(cursor);
                }
                Collections.reverse(before);

                out.println("WINDOW " + address + " radius=" + radius);
                for (Instruction instruction : before) writeInstruction(out, instruction);
                cursor = center;
                writeInstruction(out, center);
                for (int i = 0; i < radius; i++) {
                    cursor = listing.getInstructionAfter(cursor.getAddress());
                    if (cursor == null) break;
                    writeInstruction(out, cursor);
                }
            }
        }
    }

    private void writeInstruction(PrintWriter out, Instruction instruction) {
        Function function = getFunctionContaining(instruction.getAddress());
        out.println(instruction.getAddress() + "\t" +
            (function == null ? "<unclaimed>" : function.getEntryPoint() + " " + function.getName()) +
            "\t" + instruction);
    }
}
