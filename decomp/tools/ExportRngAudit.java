// @category FDNY
import java.io.FileWriter;
import java.util.LinkedHashSet;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;

public class ExportRngAudit extends GhidraScript {
    private String quote(String s) { return "\"" + s.replace("\\", "\\\\").replace("\"", "\\\"") + "\""; }
    public void run() throws Exception {
        String[] args = getScriptArgs();
        // Explicit retail routines absent from the saved analysis have to be
        // created in this read-only session before attribution can be complete.
        for (int i = 1; i < args.length; i++) {
            Address entry = toAddr(args[i]);
            if (getFunctionContaining(entry) == null) { disassemble(entry); createFunction(entry, null); }
        }
        String[] targets = {"00404930", "00404960", "00404a70", "00404ac0", "00404b20", "00404b30", "00404b50", "00498b30", "00498b50", "00498c00"};
        try (FileWriter out = new FileWriter(args[0])) {
            out.write("{\"program\":" + quote(currentProgram.getName()) + ",\"references\":[");
            boolean first = true;
            for (String target : targets) {
                Address address = toAddr(target);
                for (Reference ref : getReferencesTo(address)) {
                    Function fn = getFunctionContaining(ref.getFromAddress());
                    Instruction instruction = getInstructionAt(ref.getFromAddress());
                    if (!first) out.write(","); first = false;
                    out.write("{\"target\":" + quote(target) + ",\"site\":" + quote(ref.getFromAddress().toString()) +
                        ",\"type\":" + quote(ref.getReferenceType().toString()) + ",\"function\":" +
                        (fn == null ? "null" : quote(fn.getEntryPoint().toString())) + ",\"instruction\":" +
                        (instruction == null ? "null" : quote(instruction.toString())) + "}");
                }
            }
            out.write("]}\n");
        }
    }
}
