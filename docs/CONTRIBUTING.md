# MPQ Android Tool - Contributing Guide

## Getting Started

### Fork and Clone
```bash
git clone https://github.com/YOUR_USERNAME/mpq-android-tool.git
cd mpq-android-tool
git remote add upstream https://github.com/hooligan97/mpq-android-tool.git
```

### Create Feature Branch
```bash
git checkout -b feature/your-feature-name
```

## Code Style Guidelines

### C++ Style (Google C++ Style Guide)

**File naming:**
```cpp
// Headers: snake_case.h
// Implementation: snake_case.cpp
mpq_reader.h
mpq_reader.cpp
```

**Class naming:**
```cpp
class MPQReader {  // PascalCase
    // ...
};
```

**Function naming:**
```cpp
void open_file();  // snake_case
bool is_valid();   // snake_case with is_/has_/can_ prefix for booleans
```

**Variable naming:**
```cpp
int file_size;           // snake_case
const int MAX_SIZE = 100; // UPPER_SNAKE_CASE for constants
```

**Indentation:**
- Use 4 spaces (not tabs)
- 80 character line limit

### Kotlin Style (Kotlin Conventions)

**File naming:**
```kotlin
// One public class per file
MPQReader.kt
MPQWriter.kt
```

**Class naming:**
```kotlin
class MPQReader {  // PascalCase
    // ...
}
```

**Function naming:**
```kotlin
fun openFile() {}  // camelCase
fun isValid(): Boolean {}  // camelCase with is_/has_/can_ prefix
```

**Variable naming:**
```kotlin
val fileSize: Int = 0  // camelCase
var isOpen = false     // camelCase for vars
```

## Development Workflow

### 1. Create Feature Branch
```bash
git checkout -b feature/mpq-file-search
```

### 2. Make Changes
- Keep commits atomic and focused
- Write descriptive commit messages
- Reference issues in commits: `Fixes #123`

### 3. Test Locally
```bash
# Build debug version
./gradlew assembleDebug

# Run tests
./gradlew testDebug

# Run on device
./gradlew installDebug
```

### 4. Push Changes
```bash
git push origin feature/mpq-file-search
```

### 5. Create Pull Request
- Write clear PR title and description
- Link related issues
- Request reviews from maintainers
- Address review comments

## Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes
- `refactor`: Code refactoring
- `test`: Test additions/changes
- `chore`: Build, dependencies, etc.

**Example:**
```
feat(mpq-reader): add file extraction support

Implement file extraction from MPQ archives with
support for decompression using zlib. Includes
handling for different compression methods.

Fixes #42
```

## Testing

### Add Unit Tests

Create test file: `mpq-lib/src/test/java/com/mpq/android/tool/MPQReaderTest.kt`

```kotlin
class MPQReaderTest {
    @Test
    fun testOpenValidMPQ() {
        // Arrange
        val reader = MPQReader("test_file.mpq")
        
        // Act
        val result = reader.open()
        
        // Assert
        assertTrue(result)
    }
}
```

### Run Tests
```bash
# Unit tests
./gradlew testDebug

# Instrumented tests
./gradlew connectedAndroidTest

# Specific test
./gradlew testDebug --tests com.mpq.android.tool.MPQReaderTest
```

## Documentation

### Update README
When adding features, update the README.md with:
- New feature description
- How to use it
- Code examples

### Code Comments
```cpp
/// Brief description.
/// 
/// Detailed explanation if needed.
/// @param filename The file to extract
/// @param output_path Where to save the file
/// @return true if successful, false otherwise
bool extract_file(const std::string& filename, const std::string& output_path);
```

### Documentation Files
Add documentation to `docs/` directory:
- `ARCHITECTURE.md` - System design
- `BUILD.md` - Build instructions
- `API.md` - API reference
- `EXAMPLES.md` - Usage examples

## Pull Request Checklist

Before submitting a PR, ensure:

- [ ] Code follows style guidelines
- [ ] All tests pass locally
- [ ] New features have tests
- [ ] Documentation is updated
- [ ] Commit messages are descriptive
- [ ] No merge conflicts
- [ ] Branch is up-to-date with main

## Review Process

### Code Review
1. Maintainers review code for:
   - Correctness
   - Performance
   - Security
   - Style adherence
   - Test coverage

2. Respond to comments:
   - Address all feedback
   - Push fixes as new commits (don't force push)
   - Re-request review after changes

3. Approval and Merge
   - PR is approved by maintainers
   - All checks pass
   - Squash and merge to main

## Common Development Tasks

### Add New Feature

1. Create feature branch:
   ```bash
   git checkout -b feature/new-search-filter
   ```

2. Implement feature in appropriate module:
   ```bash
   # For C++ features
   mpq-lib/src/main/cpp/mpq-core/
   
   # For UI features
   app/src/main/java/com/mpq/android/tool/
   ```

3. Add tests:
   ```bash
   # Unit tests
   mpq-lib/src/test/java/
   
   # Integration tests
   mpq-lib/src/androidTest/java/
   ```

4. Update documentation

5. Submit PR

### Fix Bug

1. Create issue if not exists
2. Create branch: `git checkout -b fix/issue-123`
3. Reproduce bug with test
4. Fix implementation
5. Verify test passes
6. Submit PR

### Update Dependencies

1. Edit `build.gradle` files
2. Run tests to ensure compatibility
3. Update `docs/BUILD.md` if needed
4. Submit PR with changelog

## Development Environment

### Recommended Tools
- **IDE**: Android Studio 4.2+
- **VCS**: Git
- **Build**: Gradle 7.x
- **NDK**: r21+
- **CMake**: 3.22.1+

### IDE Configuration

**Android Studio Settings:**
1. File → Settings → Editor → Code Style
   - Set indent to 4 spaces
   - Enable "Optimize imports on the fly"
   - Enable "Reformat code on save"

2. File → Settings → Editor → Inspections
   - Enable security inspections
   - Enable performance warnings

### Git Hooks (Optional)

Create `.git/hooks/pre-commit`:
```bash
#!/bin/bash
./gradlew spotlessCheck
```

## Release Process

### Version Numbering
Follow Semantic Versioning: `MAJOR.MINOR.PATCH`

- `1.0.0` - Initial release
- `1.1.0` - New features
- `1.1.1` - Bug fixes
- `2.0.0` - Breaking changes

### Releasing
1. Update version in `build.gradle`
2. Update `CHANGELOG.md`
3. Create release branch: `git checkout -b release/v1.1.0`
4. Merge to main with tag: `git tag v1.1.0`
5. Build and publish release

## Getting Help

- **Issues**: Use GitHub Issues for bugs and features
- **Discussions**: Use GitHub Discussions for questions
- **Documentation**: Check `docs/` folder
- **Examples**: See `docs/EXAMPLES.md`

## Code of Conduct

- Be respectful and inclusive
- Provide constructive feedback
- Follow project guidelines
- Report issues appropriately

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
