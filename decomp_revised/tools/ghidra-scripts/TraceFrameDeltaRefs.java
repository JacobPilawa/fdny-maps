// Report all Ghidra references to the retail frame-delta global.
// Usage: -postScript TraceFrameDeltaRefs.java OUTPUT_PATH ADDRESS
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class TraceFrameDeltaRefs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) throw new IllegalArgumentException("Pass an output path and global address");
        Path output = Paths.get(args[0]);
        Address target = toAddr(args[1]);
        StringBuilder lines = new StringBuilder();
        for (Reference reference : currentProgram.getReferenceManager().getReferencesTo(target)) {
            Address from = reference.getFromAddress();
            Instruction instruction = currentProgram.getListing().getInstructionContaining(from);
            Function function = currentProgram.getFunctionManager().getFunctionContaining(from);
            lines.append(from).append(" ")
                 .append(reference.getReferenceType()).append(" ")
                 .append(function == null ? "<no function>" : function.getEntryPoint() + " " + function.getName())
                 .append(" :: ").append(instruction == null ? "<no instruction>" : instruction.toString())
                 .append('\n');
        }
        Files.writeString(output, lines.toString(), StandardCharsets.UTF_8);
        println("Wrote " + output + " with " + lines.toString().lines().count() + " references to " + target);
    }
}
