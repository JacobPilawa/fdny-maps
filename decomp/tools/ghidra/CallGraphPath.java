import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.symbol.Reference;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/** Search direct call edges from one function for a path to another function. */
public class CallGraphPath extends GhidraScript {
    private Address target;
    private final Set<Address> visited = new HashSet<>();
    private final List<String> path = new ArrayList<>();
    private int indirectCallSites;
    private int unresolvedDirectCalls;
    private boolean found;
    private boolean limitReached;
    private static final int MAX_FUNCTIONS = 20000;

    @Override
    public void run() throws Exception {
        String[] requested = getScriptArgs();
        if (requested.length != 2) {
            println("Usage: CallGraphPath.java <start-entry> <target-entry>");
            return;
        }

        Function start = getFunctionAt(toAddr(normalize(requested[0])));
        Function targetFunction = getFunctionAt(toAddr(normalize(requested[1])));
        if (start == null || targetFunction == null) {
            println("NO FUNCTION at start or target address");
            return;
        }
        target = targetFunction.getEntryPoint();
        println("START " + start.getEntryPoint() + " " + start.getName());
        println("TARGET " + targetFunction.getEntryPoint() + " " + targetFunction.getName());

        search(start);
        if (found) {
            println("DIRECT CALL PATH");
            for (String edge : path) {
                println("  " + edge);
            }
        } else {
            println("NO DIRECT CALL PATH FOUND");
        }
        println("DIRECT FUNCTIONS VISITED " + visited.size());
        println("INDIRECT CALL SITES NOT FOLLOWED " + indirectCallSites);
        println("UNRESOLVED DIRECT CALLS " + unresolvedDirectCalls);
        println("FUNCTION LIMIT REACHED " + limitReached);
    }

    private void search(Function function) throws Exception {
        Address entry = function.getEntryPoint();
        if (visited.contains(entry) || found) {
            return;
        }
        if (visited.size() >= MAX_FUNCTIONS) {
            limitReached = true;
            return;
        }
        visited.add(entry);

        InstructionIterator instructions = currentProgram.getListing()
            .getInstructions(function.getBody(), true);
        while (instructions.hasNext() && !found) {
            Instruction instruction = instructions.next();
            if (!instruction.getFlowType().isCall()) {
                continue;
            }
            if (instruction.getFlowType().isComputed()) {
                indirectCallSites++;
                continue;
            }

            for (Reference reference : currentProgram.getReferenceManager()
                    .getReferencesFrom(instruction.getAddress())) {
                if (!reference.getReferenceType().isCall()) {
                    continue;
                }
                Address destination = reference.getToAddress();
                Function callee = getFunctionAt(destination);
                if (callee == null) {
                    callee = getFunctionContaining(destination);
                }
                String calleeName = callee == null ? "<no function>" : callee.getName();
                String edge = function.getEntryPoint() + " " + function.getName()
                    + " @ " + instruction.getAddress() + " -> " + destination
                    + " " + calleeName;
                path.add(edge);

                if (destination.equals(target)
                        || (callee != null && callee.getEntryPoint().equals(target))) {
                    found = true;
                    return;
                }
                if (callee == null) {
                    unresolvedDirectCalls++;
                } else {
                    search(callee);
                }
                if (!found) {
                    path.remove(path.size() - 1);
                }
            }
        }
    }

    private String normalize(String text) {
        return text.startsWith("0x") || text.startsWith("0X")
            ? text.substring(2) : text;
    }
}
