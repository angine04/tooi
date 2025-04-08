#include "catch2.hpp"
#include "tooi/cli/args_parser.h"

using namespace tooi::cli;

TEST_CASE("ArgsParser Basic Initialization", "[args_parser]") {
    ArgsParser parser;
    INFO("Checking default state");
    REQUIRE(parser.get_mode() == RunMode::REPL);
    REQUIRE(parser.get_filename().empty());
    REQUIRE_FALSE(parser.is_verbose());
}

TEST_CASE("ArgsParser Help Mode", "[args_parser]") {
    ArgsParser parser;
    
    SECTION("Long form help") {
        INFO("Checking --help argument");
        const char* args[] = {"program", "--help"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::HELP);
    }
    
    SECTION("Short form help") {
        INFO("Checking -h argument");
        const char* args[] = {"program", "-h"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::HELP);
    }
    
    SECTION("Help overrides other options") {
        INFO("Checking help overrides other options");
        const char* args[] = {"program", "--verbose", "test.tooi", "--help"};
        parser.parse(4, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::HELP);
    }
}

TEST_CASE("ArgsParser Version Mode", "[args_parser]") {
    ArgsParser parser;
    
    SECTION("Long form version") {
        INFO("Checking --version argument");
        const char* args[] = {"program", "--version"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::VERSION);
    }
    
    SECTION("Short form version") {
        INFO("Checking -v argument");
        const char* args[] = {"program", "-v"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::VERSION);
    }
    
    SECTION("Version overrides other options") {
        INFO("Checking version overrides other options");
        const char* args[] = {"program", "--verbose", "test.tooi", "--version"};
        parser.parse(4, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::VERSION);
    }
}

TEST_CASE("ArgsParser Verbose Mode", "[args_parser]") {
    ArgsParser parser;
    
    SECTION("Long form verbose") {
        INFO("Checking --verbose argument");
        const char* args[] = {"program", "--verbose"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.is_verbose());
        REQUIRE(parser.get_mode() == RunMode::REPL);  // Should stay in REPL mode
    }
    
    SECTION("Short form verbose") {
        INFO("Checking -V argument");
        const char* args[] = {"program", "-V"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.is_verbose());
        REQUIRE(parser.get_mode() == RunMode::REPL);  // Should stay in REPL mode
    }
    
    SECTION("Verbose with file") {
        INFO("Checking verbose with file argument");
        const char* args[] = {"program", "--verbose", "test.tooi"};
        parser.parse(3, const_cast<char**>(args));
        REQUIRE(parser.is_verbose());
        REQUIRE(parser.get_mode() == RunMode::FILE);
        REQUIRE(parser.get_filename() == "test.tooi");
    }
}

TEST_CASE("ArgsParser File Mode", "[args_parser]") {
    ArgsParser parser;
    
    SECTION("Single file") {
        INFO("Checking single file argument");
        const char* args[] = {"program", "test.tooi"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::FILE);
        REQUIRE(parser.get_filename() == "test.tooi");
    }
    
    SECTION("File with verbose") {
        INFO("Checking file with verbose flag");
        const char* args[] = {"program", "--verbose", "test.tooi"};
        parser.parse(3, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::FILE);
        REQUIRE(parser.get_filename() == "test.tooi");
        REQUIRE(parser.is_verbose());
    }
    
    SECTION("Multiple files error") {
        INFO("Checking multiple files error");
        const char* args[] = {"program", "test1.tooi", "test2.tooi"};
        parser.parse(3, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::ERROR);
    }
}

TEST_CASE("ArgsParser Error Handling", "[args_parser]") {
    ArgsParser parser;
    
    SECTION("Unknown option") {
        INFO("Checking unknown option error");
        const char* args[] = {"program", "--unknown"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::ERROR);
    }
    
    SECTION("Invalid short option") {
        INFO("Checking invalid short option error");
        const char* args[] = {"program", "-x"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::ERROR);
    }
}

TEST_CASE("ArgsParser Default REPL Mode", "[args_parser]") {
    ArgsParser parser;
    
    SECTION("No arguments") {
        INFO("Checking no arguments defaults to REPL");
        const char* args[] = {"program"};
        parser.parse(1, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::REPL);
        REQUIRE(parser.get_filename().empty());
    }
    
    SECTION("Only verbose") {
        INFO("Checking only verbose flag defaults to REPL");
        const char* args[] = {"program", "--verbose"};
        parser.parse(2, const_cast<char**>(args));
        REQUIRE(parser.get_mode() == RunMode::REPL);
        REQUIRE(parser.get_filename().empty());
        REQUIRE(parser.is_verbose());
    }
} 