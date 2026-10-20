// Find instructions that write a memory operand containing a selected x86
// displacement. This is a broad candidate scan; it does not identify aliases
// or prove that a matching base register is a particular object.
// Usage: -postScript FindOffsetWrites.java OUTPUT_PATH 0x10 [0x20 ...]
import ghidra.app.script.GhidraScript;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.pcode.PcodeOp;
import ghidra.program.model.scalar.Scalar;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;

public class FindOffsetWrites extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException("Pass an output path and one or more offsets");
        }

        Path output = Paths.get(args[0]);
        Set<Long> wanted = new HashSet<>();
        for (int i = 1; i < args.length; i++) {
            String raw = args[i].toLowerCase(Locale.ROOT).replaceFirst("^0x", "");
            wanted.add(Long.parseUnsignedLong(raw, 16));
        }

        StringBuilder lines = new StringBuilder();
        InstructionIterator iterator = currentProgram.getListing().getInstructions(true);
        long matches = 0;
        while (iterator.hasNext()) {
            Instruction instruction = iterator.next();
            boolean storesMemory = false;
            for (PcodeOp operation : instruction.getPcode()) {
                if (operation.getOpcode() == PcodeOp.STORE) {
                    storesMemory = true;
                    break;
                }
            }
            if (!storesMemory) continue;

            for (int operand = 0; operand < instruction.getNumOperands(); operand++) {
                boolean hasRegister = false;
                boolean hasWantedDisplacement = false;
                for (Object object : instruction.getOpObjects(operand)) {
                    if (object instanceof Register) hasRegister = true;
                    if (object instanceof Scalar && wanted.contains(((Scalar)object).getUnsignedValue())) {
                        hasWantedDisplacement = true;
                    }
                }
                if (!hasRegister || !hasWantedDisplacement) continue;

                Function function = currentProgram.getFunctionManager()
                    .getFunctionContaining(instruction.getAddress());
                lines.append(instruction.getAddress()).append(" ")
                     .append(function == null ? "<no function>" :
                         function.getEntryPoint() + " " + function.getName())
                     .append(" operand=").append(operand)
                     .append(" :: ").append(instruction).append('\n');
                matches++;
            }
        }

        Files.writeString(output, lines.toString(), StandardCharsets.UTF_8);
        println("Wrote " + output + " with " + matches + " candidate writes");
    }
}
