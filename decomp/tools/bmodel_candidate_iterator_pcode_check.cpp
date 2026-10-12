#include <json-c/json.h>

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../src/bmodel_candidate_iterator.h"

namespace {
using J = json_object*;

struct FixtureMemory final : BModelCandidateIterator::Memory {
    std::vector<std::pair<uint32_t, uint32_t>> words;

    uint32_t read32(uint32_t address) const override {
        for (auto it = words.rbegin(); it != words.rend(); ++it)
            if (it->first == address) return it->second;
        return 0;
    }

    void put(uint32_t address, uint32_t value) { words.emplace_back(address, value); }
};

J field(J object, const char* key) {
    J value = nullptr;
    if (!json_object_object_get_ex(object, key, &value))
        throw std::runtime_error(std::string("Missing field: ") + key);
    return value;
}

uint32_t u32(J value) {
    return static_cast<uint32_t>(json_object_get_uint64(value));
}

std::string string(J value) {
    return json_object_get_string(value);
}

struct Check {
    int mismatches = 0;

    void require(bool condition, const std::string& sample, const char* detail) {
        if (!condition) {
            ++mismatches;
            std::cerr << sample << " mismatch: " << detail << "\n";
        }
    }
};

void compareSample(J row, uint32_t managerGlobal, Check& check) {
    const std::string name = string(field(row, "name"));
    FixtureMemory memory;
    const uint32_t headAddress = 0x20010054u + 0x10u;
    const uint32_t firstNode = 0x20020010u;
    J input = field(row, "controlled_input_objects");
    const size_t count = json_object_array_length(input);
    std::vector<uint32_t> nodes;
    std::vector<uint32_t> objects;
    for (size_t i = 0; i < count; ++i) {
        objects.push_back(u32(json_object_array_get_idx(input, static_cast<int>(i))));
        nodes.push_back(firstNode + static_cast<uint32_t>(i) * 8u);
    }
    for (size_t i = 0; i < count; ++i) {
        memory.put(nodes[i], objects[i]);
        memory.put(nodes[i] + 4, i + 1 < count ? nodes[i + 1] : 0);
    }
    memory.put(headAddress, nodes.empty() ? 0 : nodes.front());

    BModelCandidateIterator::Cursor cursor;
    cursor.list = 0xdeadbeefu;
    cursor.node = 0xcafebabeu;
    unsigned registrations = 0;
    uint32_t registeredList = 0;
    const BModelCandidateIterator::Cursor* registeredCursor = nullptr;
    BModelCandidateIterator::initialize(
        cursor, managerGlobal, memory,
        [&](uint32_t list, BModelCandidateIterator::Cursor& registered) {
            ++registrations;
            registeredList = list;
            registeredCursor = &registered;
        });

    const J atInit = field(row, "cursor_words_at_00481BA0");
    const J expectedOrder = field(row, "controlled_expected_order");
    check.require(json_object_get_boolean(field(row, "cursor_zeroed_by_00481C00")) &&
                      json_object_array_length(atInit) == 2 &&
                      u32(json_object_array_get_idx(atInit, 0)) == 0 &&
                      u32(json_object_array_get_idx(atInit, 1)) == 0,
                  name, "original 00481C00 cursor zeroing before nested initializer");
    check.require(registrations == 1 && registeredCursor == &cursor,
                  name, "one registration callback receives this cursor");
    check.require(registeredList == u32(field(row, "registration_receiver")),
                  name, "registration list address matches original boundary");
    check.require(cursor.list == u32(field(row, "list_address")),
                  name, "native list address matches original cursor output");
    check.require(cursor.node == u32(field(row, "first_node")),
                  name, "native first-node value matches original cursor output");

    std::vector<uint32_t> actualOrder;
    BModelCandidateIterator::each(cursor, memory,
                                  [&](uint32_t object) { actualOrder.push_back(object); });
    check.require(actualOrder.size() == json_object_array_length(expectedOrder),
                  name, "controlled linked-list traversal length");
    for (size_t i = 0; i < actualOrder.size() &&
                       i < json_object_array_length(expectedOrder); ++i) {
        check.require(actualOrder[i] ==
                          u32(json_object_array_get_idx(expectedOrder, static_cast<int>(i))),
                      name, "controlled linked-list pointer identity/order");
    }
}
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 2)
            throw std::runtime_error("Usage: bmodel_candidate_iterator_pcode_check fixture.json");
        J fixture = json_object_from_file(argv[1]);
        if (!fixture) throw std::runtime_error("Cannot read candidate iterator fixture");
        if (string(field(fixture, "exe_sha256")) !=
            "4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")
            throw std::runtime_error("Candidate iterator fixture executable hash mismatch");

        Check check;
        J samples = field(fixture, "samples");
        for (size_t i = 0; i < json_object_array_length(samples); ++i)
            compareSample(json_object_array_get_idx(samples, static_cast<int>(i)),
                          0x20010000u, check);

        J nullRow = field(fixture, "null_list_sentinel_sample");
        compareSample(nullRow, 0xffffffb0u, check);
        check.require(u32(field(nullRow, "registration_receiver")) == 0,
                      string(field(nullRow, "name")),
                      "original null-list registration receiver is explicitly captured");

        std::cout << "BModel candidate iterator: P-code cursor cases="
                  << json_object_array_length(samples) + 1
                  << ", mismatches=" << check.mismatches
                  << " (traversal order checked against controlled/source-derived links)\n";
        json_object_put(fixture);
        return check.mismatches ? 1 : 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << "\n";
        return 2;
    }
}
