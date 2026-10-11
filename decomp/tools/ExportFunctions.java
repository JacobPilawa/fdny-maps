// Export selected functions and nearby evidence from a Ghidra project.
// @category FDNY

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.LinkedHashSet;
import java.util.Set;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.StringDataInstance;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.symbol.Reference;

public class ExportFunctions extends GhidraScript {
    private static final String[] KEYWORDS = {
        "VictimRescue", "PlayerRescue", "FireRescue", "Jump", "FootStep",
        "goggles", "extinguisher", "touchhose", "climb", "SnapToFloor",
        "All of the survivors", "setrescuebot", "CROUCH", "MOVE_FORWARD",
        "AxeTool", "ExtinguisherTool", "HoseTool", "SprayTime", "Chop",
        "AxeSwing", "AxeHitWood", "HoseSpray", "pickup", "openaway",
        "RCS_VictimAltSpot", "RCS_VictimRescue", "RCS_FireRescue"
    };

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            throw new IllegalArgumentException("usage: ExportFunctions.java OUTPUT_DIR [ADDRESS ...]");
        }
        File outputDir = new File(args[0]);
        if (!outputDir.exists() && !outputDir.mkdirs()) {
            throw new IllegalStateException("cannot create " + outputDir);
        }

        Set<Function> selected = new LinkedHashSet<>();
        boolean scanKeywords = true;
        for (int i = 1; i < args.length; i++) {
            if ("--direct-only".equals(args[i])) {
                scanKeywords = false;
                continue;
            }
            if (args[i].startsWith("callers:")) {
                Address target = toAddr(args[i].substring("callers:".length()));
                for (Reference ref : getReferencesTo(target)) {
                    Function caller = getFunctionContaining(ref.getFromAddress());
                    if (caller != null) selected.add(caller);
                }
                continue;
            }
            if (args[i].startsWith("range:")) {
                String[] bounds = args[i].split(":");
                Address first = toAddr(bounds[1]);
                Address last = toAddr(bounds[2]);
                FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(first, true);
                while (functions.hasNext()) {
                    Function candidate = functions.next();
                    if (candidate.getEntryPoint().compareTo(last) > 0) break;
                    selected.add(candidate);
                }
                continue;
            }
            Address address = toAddr(args[i].replace("0x", ""));
            Function function = getFunctionContaining(address);
            // Some important retail routines have no symbol/function boundary
            // after auto-analysis.  When the supplied address is executable,
            // make the boundary explicit so Ghidra can decompile it rather
            // than silently treating the address like data.
            if (function == null && currentProgram.getMemory().getBlock(address) != null
                    && currentProgram.getMemory().getBlock(address).isExecute()) {
                disassemble(address);
                function = createFunction(address, null);
            }
            if (function != null) {
                selected.add(function);
            } else {
                // Passing a global/data address exports every function that
                // references it, which is useful for constants and IAT slots.
                for (Reference reference : getReferencesTo(address)) {
                    Function referencing = getFunctionContaining(reference.getFromAddress());
                    if (referencing != null) selected.add(referencing);
                }
            }
        }

        if (scanKeywords) {
            DataIterator dataIterator = currentProgram.getListing().getDefinedData(true);
            while (dataIterator.hasNext()) {
                Data data = dataIterator.next();
                if (data.hasStringValue()) {
                    String value = StringDataInstance.getStringDataInstance(data).getStringValue();
                    if (value != null && containsKeyword(value)) {
                        for (Reference reference : getReferencesTo(data.getAddress())) {
                            Function function = getFunctionContaining(reference.getFromAddress());
                            if (function != null) selected.add(function);
                        }
                    }
                }
            }
        }

        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        try (BufferedWriter index = new BufferedWriter(new FileWriter(new File(outputDir, "index.tsv")))) {
            index.write("entry\tname\tsize\n");
            for (Function function : selected) {
                monitor.checkCancelled();
                String address = function.getEntryPoint().toString();
                String safeName = function.getName().replaceAll("[^A-Za-z0-9_.-]", "_");
                File destination = new File(outputDir, address + "_" + safeName + ".c");
                DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
                try (BufferedWriter writer = new BufferedWriter(new FileWriter(destination))) {
                    writer.write("/* Ghidra decompilation; names/types are provisional.\n");
                    writer.write(" * Entry: " + address + "\n");
                    writer.write(" * Function: " + function.getName() + "\n */\n\n");
                    if (result.decompileCompleted()) {
                        writer.write(result.getDecompiledFunction().getC());
                    } else {
                        writer.write("/* decompilation failed: " + result.getErrorMessage() + " */\n");
                    }
                }
                index.write(address + "\t" + function.getName() + "\t" + function.getBody().getNumAddresses() + "\n");
            }
        } finally {
            decompiler.dispose();
        }
        println("Exported " + selected.size() + " functions to " + outputDir);
    }

    private boolean containsKeyword(String value) {
        String lower = value.toLowerCase();
        for (String keyword : KEYWORDS) {
            if (lower.contains(keyword.toLowerCase())) return true;
        }
        return false;
    }
}
