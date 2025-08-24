# PHP DuckDB Extension

In‑process analytics for PHP powered by [DuckDB](https://duckdb.org).\
This extension delivers fast on‑disk and in‑memory SQL, **prepared statements**, streaming fetch, bulk appends, and — most importantly — **first‑class PHP User‑Defined Functions (UDFs)** so you can call your own PHP code directly from SQL.

> **Highlights**
> - **PHP UDFs**: Register PHP callables and use them inside SQL.  
> - **Prepared statements** (positional parameters).  
> - **Streaming result** iteration over millions of rows.  
> - **Bulk inserts** via Appender (with `flush()` for immediate visibility).  
> - **Flexible fetching** (objects by default, associative arrays optional).  
> - **Clear exceptions** via `Fnvoid\DuckDB\Exception`.

---

## Table of Contents
- [Installation](#installation)
- [Creating a Connection](#creating-a-connection)
- [Query Execution](#query-execution)
- [Prepared Statements](#prepared-statements)
- [Fetching Data](#fetching-data)
- [Appender](#appender)
- [User Defined Functions (UDFs)](#user-defined-functions-udfs)
- [Errors & Exceptions](#errors--exceptions)
- [License](#license)

---

## Installation

### DuckDB PHP Extension Installation Guide

This guide explains how to install DuckDB and compile the PHP extension on Linux. Pre-built binaries will be provided in the future for easier installation.

---

#### 1. Install DuckDB

##### x86\_64

```bash
wget https://github.com/duckdb/duckdb/releases/download/v1.3.2/libduckdb-linux-amd64.zip
sudo unzip libduckdb-linux-amd64.zip duckdb.h -d /usr/local/include/
sudo unzip libduckdb-linux-amd64.zip libduckdb.so -d /usr/local/lib/
rm libduckdb-linux-amd64.zip
sudo ldconfig
```

##### arm64

```bash
wget https://github.com/duckdb/duckdb/releases/download/v1.3.2/libduckdb-linux-arm64.zip
sudo unzip libduckdb-linux-arm64.zip duckdb.h -d /usr/local/include/
sudo unzip libduckdb-linux-arm64.zip libduckdb.so -d /usr/local/lib/
rm libduckdb-linux-arm64.zip
```

> ⚠️ Make sure the architecture of the DuckDB binary matches your system.

---

#### 2. Install Necessary Build Tools

##### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y build-essential autoconf automake bison flex re2c gdb \
libtool make pkg-config valgrind git libxml2-dev libsqlite3-dev php-dev \
zlib1g-dev libssl-dev
```

##### RedHat / Fedora

```bash
sudo dnf install -y gcc gcc-c++ binutils glibc-devel autoconf automake bison \
flex re2c gdb libtool make pkgconf valgrind git libxml2-devel libsqlite3x-devel \
zlib-devel openssl-devel php-devel
```

> These packages cover common compilation requirements. Additional dependencies may be required depending on your PHP configuration.

---

#### 3. Compile the PHP Extension

```bash
git clone https://github.com/fnvoid64/php-duckdb.git
cd php-duckdb
phpize
./configure
make
make test
sudo make install
```

> After installation, enable the extension in your `php.ini`:

```ini
extension=duckdb.so
```

---

#### 4. Future Plans

Pre-built binaries for popular Linux distributions and architectures will be published soon, making installation much simpler without compiling from source.


## Creating a Connection

### In‑memory
```php
try {
    $db = new Fnvoid\DuckDB\DuckDB(); // In‑memory database
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

### File‑backed
```php
try {
    $db = new Fnvoid\DuckDB\DuckDB('/path/to/db');
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

### With Configuration
You can pass DuckDB configuration either as the second argument or via a named `array $config`.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB('/path/to/db', [
        'threads' => 8,
    ]);

    // or using a named argument:
    $db = new Fnvoid\DuckDB\DuckDB(config: [
        'threads' => 8,
    ]);
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## Query Execution
`Fnvoid\DuckDB\DuckDB::query()` returns a `Fnvoid\DuckDB\Result` object that you can fetch from.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB(); // In‑memory
    $res = $db->query("CREATE TABLE test_table (id INT, name VARCHAR, age INT)");
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## Prepared Statements
Prepared statements currently support **positional parameters only**.  
`Fnvoid\DuckDB\DuckDB::prepare()` returns a `Fnvoid\DuckDB\Statement`.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB();
    $stmt = $db->prepare("SELECT * FROM test_table WHERE id = ?");
    $res  = $stmt->execute([1]);
    $row  = $res->fetchOne(); // Fetch a single row
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## Fetching Data

### 1) Stream with `iterate()` (recommended)
Efficient, lazy iteration suitable for very large result sets.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB();
    $stmt = $db->prepare("SELECT * FROM test_table WHERE status = ?");
    $res  = $stmt->execute([1]);

    foreach ($res->iterate() as $row) {
        // Process row
    }
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

### 2) Get everything with `fetchAll()`
Best for small result sets.

```php
$res = $stmt->execute([1])->fetchAll();
```

### 3) Get a single row with `fetchOne()`
```php
$res = $stmt->execute([1])->fetchOne();
```

### Fetch modes
All fetchers (`iterate()`, `fetchAll()`, `fetchOne()`) accept an optional boolean `$assoc`:
- `false` (default) → **objects**
- `true` → **associative arrays**

---

## Appender
Use the **Appender** for bulk inserts. Values are positional and must match the table schema order.  
Data is **not immediately visible** to queries until you call `flush()`.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB();
    $appender = $db->createAppender('table_name');

    foreach ($someLocalData as $row) {
        $appender->appendRow([
            $row->id,
            $row->name,
            $row->age,
        ]);
    }

    // Make appended data visible to queries
    $appender->flush();

    $res = $db->query("SELECT * FROM table_name");
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## User Defined Functions (UDFs)
Run your own **PHP code inside DuckDB SQL**. This is a major feature for many workloads (custom transforms, business rules, domain logic, etc.).

> **Requirements & Notes**
> - UDFs require **single‑threaded** execution: set `threads = 1` in config.  
> - Supported return types: `string`, `int`, `float`, `double`, `null`, `bool`.  
> - Register PHP functions by name; then call them from SQL.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB(config: [
        'threads' => 1, // Required for UDFs
    ]);

    // Register a PHP function
    $db->registerFunction('php_upper', fn (string $s): string => strtoupper($s));

    $stmt = $db->prepare("
        SELECT id, name, php_upper(name) AS name_upper
        FROM test_table
        WHERE status = ?
    ");

    $row = $stmt->execute([1])->fetchOne();
} catch (Fnvoid\DuckDB\Exception $e) {
    echo $e->getMessage();
}
```

---

## Errors & Exceptions
All failures throw `Fnvoid\DuckDB\Exception`. Wrap calls in `try/catch` to handle errors gracefully.

```php
try {
    $db = new Fnvoid\DuckDB\DuckDB('/invalid/path');
} catch (Fnvoid\DuckDB\Exception $e) {
    // Log / rethrow / surface message
    error_log($e->getMessage());
}
```

---

