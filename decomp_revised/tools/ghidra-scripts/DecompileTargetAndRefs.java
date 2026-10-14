// Print one target function, its disassembly, and direct code/data references.
// Usage: -postScript DecompileTargetAndRefs.java 00478900
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;

public class DecompileTargetAndRefs extends ghidra.app.script.GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) throw new IllegalArgumentException("Pass one or more hexadecimal addresses");
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        println("--- FUNCTIONS 0049C000..0049D400 ---");
        Address rangeStart = toAddr("0049C000");
        Address rangeEnd = toAddr("0049D400");
        for (Function nearby : currentProgram.getFunctionManager().getFunctions(rangeStart, true)) {
            if (nearby.getEntryPoint().compareTo(rangeEnd) >= 0) break;
            println(nearby.getEntryPoint() + " " + nearby.getName());
        }
        for (String argument : args) {
            Address target = toAddr(argument.startsWith("0x") ? argument : "0x" + argument);
            Function function = getFunctionAt(target);
            if (function == null) function = getFunctionContaining(target);
            if (function == null) {
                println("DATA REFERENCES TO " + target);
                ReferenceIterator dataReferences = currentProgram.getReferenceManager().getReferencesTo(target);
                while (dataReferences.hasNext()) {
                    Reference reference = dataReferences.next();
                    Function owner = getFunctionContaining(reference.getFromAddress());
                    println(reference.getFromAddress() + " " + reference.getReferenceType() +
                            " owner=" + (owner == null ? "<none>" : owner.getEntryPoint() + " " + owner.getName()));
                }
                continue;
            }

            println("FUNCTION " + function.getEntryPoint() + " " + function.getName());
            DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
            if (result == null || !result.decompileCompleted()) {
                println("DECOMPILE_ERROR " + (result == null ? "null result" : result.getErrorMessage()));
            } else {
                println("--- DECOMPILED ---");
                println(result.getDecompiledFunction().getC());
            }

            println("--- DISASSEMBLY ---");
            for (Instruction instruction : currentProgram.getListing().getInstructions(function.getBody(), true)) {
                println(instruction.getAddress() + "  " + instruction);
            }
            println("--- REFERENCES TO ENTRY ---");
            ReferenceIterator references = currentProgram.getReferenceManager()
                    .getReferencesTo(function.getEntryPoint());
            while (references.hasNext()) {
                Reference reference = references.next();
                println(reference.getFromAddress() + " " + reference.getReferenceType());
            }
        }
        decompiler.dispose();
    }
}
