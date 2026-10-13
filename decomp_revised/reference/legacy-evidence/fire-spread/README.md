# Reused FireRescue work from the previous workspace

These files retain the prior fire-mechanics audit because it contains substantial
work that would be wasteful to repeat. The two legacy overview documents cover
authored route spread, random spread, suppression, recovery, exposure, damage,
RNG, and player passout. Additional message-transaction notes retain the old
source-order and arithmetic analysis.

The prior notes refer to a companion `evidence.zip`, which is not present in
the previous-work directory. The existing GitHub `decomp/` archive also held a
source-hash-tagged fire-suppression oracle and model; I copied those selected
artifacts and several relevant old Ghidra exports here. They have been compared
with fresh output from the current program where noted in
[`tracker/evidence/fire-spread/README.md`](../../../tracker/evidence/fire-spread/README.md).
Treat old assembly-only or runtime-scheduling claims as historical until they
are rechecked against saved current evidence or a retail capture.

No original executable or game assets are stored in this archive. The copied
oracle/model are derived research artifacts, not the original game source.
