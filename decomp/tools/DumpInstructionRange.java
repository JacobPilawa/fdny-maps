// Export already-decoded instructions within an address range.
// @category FDNY

import java.io.FileWriter;
import java.io.PrintWriter;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

public class DumpInstructionRange extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 3) {
            throw new IllegalArgumentException(
                "usage: DumpInstructionRange.java OUTPUT_FILE START_ADDRESS END_ADDRESS");
        }
        Address first = toAddr(args[1].replace("0x", ""));
        Address last = toAddr(args[2].replace("0x", ""));
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            out.println("address\tinstruction");
            InstructionIterator instructions = currentProgram.getListing().getInstructions(first, true);
            while (instructions.hasNext()) {
                monitor.checkCancelled();
                Instruction instruction = instructions.next();
                if (instruction.getAddress().compareTo(last) > 0) break;
                out.println(instruction.getAddress() + "\t" + instruction);
            }
        }
    }
}
