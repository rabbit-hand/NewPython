# Security Policy

**Last Updated: May 23, 2026**

---

## ⚠️ Security Notice

**This is an UNOFFICIAL and EXPERIMENTAL project. It has NOT undergone professional security audits. Use at your own risk.**

---

## Supported Versions

| Version | Supported |
|---------|-----------|
| NewPython v2 (current) | ✅ Experimental Support |

**Note**: As this is an experimental project, security support is provided on a best-effort basis only.

---

## Reporting a Vulnerability

If you discover a security vulnerability, please report it responsibly:

### How to Report

1. **Do NOT** create a public GitHub issue
2. Send an email to: security@example.com (placeholder - update with actual contact)
3. Include:
   - Description of the vulnerability
   - Steps to reproduce
   - Potential impact
   - Suggested fix (if known)

### Response Timeline

- Initial response: Within 48 hours
- Detailed response: Within 7 days
- Fix release: As soon as feasible (experimental project)

---

## Security Features

### Implemented Security Measures

#### 1. File Validation
- **File Size Limit**: Maximum 10MB to prevent DoS attacks
- **Line Length Limit**: Maximum 10,000 characters per line
- **File Extension Whitelist**: Only `.newpy`, `.py`, `.txt` allowed
- **Empty File Detection**: Rejects empty files

#### 2. Path Validation
- **Path Traversal Protection**: Resolves absolute paths to prevent directory traversal
- **File Existence Check**: Verifies file exists before processing
- **File Type Verification**: Ensures path points to a file, not a directory

#### 3. Encoding Validation
- **UTF-8 Enforcement**: All source files must be UTF-8 encoded
- **Encoding Error Handling**: Graceful error messages for encoding issues

#### 4. Input Validation
- **Source Code Validation**: Checks for empty source code
- **Line Count Limit**: Maximum 100,000 lines to prevent memory exhaustion
- **Comprehensive Error Handling**: Clear error messages for validation failures

#### 5. Parser Security
- **Regex Safety**: Uses compiled regex patterns to prevent ReDoS
- **Memory Management**: Processes files line-by-line to limit memory usage
- **Exception Handling**: Catches and reports errors gracefully

---

## Security Best Practices for Users

### When Using NewPython

1. **Never process untrusted files** from unknown sources
2. **Review generated Python code** before execution
3. **Use in isolated environments** (containers, VMs)
4. **Keep your Python interpreter updated**
5. **Don't use for production systems**

### File Handling

- Only process files from trusted sources
- Scan files for malware before processing
- Use virus scanners on downloaded files
- Verify file integrity after download

### Code Execution

- Always review generated Python code before running
- Test in sandboxed environments first
- Never run with elevated privileges
- Monitor resource usage during execution

---

## Known Limitations

### Current Security Limitations

1. **No Code Signing**: Files are not cryptographically verified
2. **No Sandboxing**: Generated code runs with same privileges as Python
3. **No Static Analysis**: No security scanning of generated code
4. **No Runtime Protection**: No runtime security monitoring
5. **Limited Audit Trail**: No comprehensive logging of operations

### Future Security Improvements

- [ ] Add file hashing/integrity verification
- [ ] Implement code signing support
- [ ] Add static analysis for generated code
- [ ] Implement sandboxing options
- [ ] Add comprehensive logging
- [ ] Security audit by professionals

---

## Threat Model

### Assumed Threats

This project protects against:

- **Path Traversal Attacks**: Through path validation
- **Denial of Service (DoS)**: Through file size and line limits
- **Memory Exhaustion**: Through line count limits
- **Encoding Attacks**: Through UTF-8 enforcement
- **Basic Input Validation**: Through comprehensive checks

### Not Protected Against

This project does NOT protect against:

- **Malicious Generated Code**: Users must review output
- **Runtime Attacks**: Standard Python security applies
- **Social Engineering**: User education required
- **Supply Chain Attacks**: Verify sources manually
- **Advanced Persistent Threats**: Not designed for high-security environments

---

## Dependencies

### External Dependencies

This project uses only Python standard library:

- `re` - Regular expressions
- `sys` - System operations
- `pathlib` - Path handling
- `os` - Operating system interface
- `typing` - Type hints

### Dependency Security

- No external dependencies to reduce attack surface
- Standard library components maintained by Python core team
- Regular Python updates recommended

---

## Development Security

### For Contributors

When contributing to NewPython:

1. **Follow security best practices** in code
2. **Add tests** for security-related features
3. **Document security implications** of changes
4. **Review regex patterns** for ReDoS vulnerabilities
5. **Validate all inputs** in new features
6. **Handle errors gracefully** without exposing sensitive information

### Code Review Checklist

- [ ] Input validation implemented
- [ ] Error handling doesn't leak information
- [ ] No hardcoded credentials
- [ ] Regex patterns are safe
- [ ] File operations are validated
- [ ] Memory usage is bounded
- [ ] Tests cover security cases

---

## Compliance

### Legal Compliance

This project is provided for educational and experimental purposes only. Users are responsible for:

- Complying with local laws and regulations
- Obtaining necessary permissions for use
- Ensuring appropriate security measures
- Not using for illegal activities

### Data Protection

- No user data is collected
- No telemetry or analytics
- No network communications
- Files are processed locally only

---

## Incident Response

### Security Incident Process

In case of a security incident:

1. **Assessment**: Determine severity and impact
2. **Containment**: Limit damage if possible
3. **Communication**: Notify users if necessary
4. **Remediation**: Develop and deploy fix
5. **Post-Mortem**: Document and learn

### Severity Levels

- **Critical**: Immediate fix required
- **High**: Fix within 7 days
- **Medium**: Fix within 30 days
- **Low**: Fix in next release

---

## Contact

### Security Questions

For security-related questions:
- Email: security@example.com (placeholder)
- GitHub Issues: Use label `security` (non-sensitive only)

### Non-Security Issues

For bugs, features, or general questions:
- GitHub Issues: https://github.com/rabbit-hand/NewPython/issues
- Discussions: https://github.com/rabbit-hand/NewPython/discussions

---

## Disclaimer

**This security policy is provided for informational purposes only. As this is an experimental project, no security guarantees are made or implied. Users assume all risks associated with using this software.**

---

**Remember: This is experimental software. Implement your own security measures appropriate for your use case.**
