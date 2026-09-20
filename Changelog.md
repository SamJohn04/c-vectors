# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0] - 2026-09-19

### Added

- vector_pop to return the last element after deleting it
- vector_get to safely get a value (returning 0 if it does not exist)
- tests for both of these
- revamp the test suite
- (try to) make all functions run without crashing even when vector has value NULL
