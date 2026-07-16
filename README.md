# Teeworlds MRPG Server

MRPG server based on the Teeworlds/DDNet 0.6 codebase, with a compatibility
bridge for the custom
[Teeworlds MRPG 0.7 Client](https://github.com/sqwinixxx/Teeworlds-MRPG-Client).

## Compatibility

| Client | Support |
| --- | --- |
| [Teeworlds-MRPG-Client](https://github.com/sqwinixxx/Teeworlds-MRPG-Client) | Full MRPG support (recommended) |
| Vanilla Teeworlds 0.6/DDNet client | Base protocol connection is supported, but the custom MRPG UI is unavailable |
| Vanilla Teeworlds 0.7 client | Sixup connection may work, but the custom MRPG UI and messages are unavailable |

The server keeps its native 0.6 path and adds a dedicated 0.7 compatibility
path. The bridge translates connection packets, map transfer, snapshots,
standard game messages, client identity, game information and MRPG-specific
handshake/account messages. It also translates 0.6 game-state flags instead of
copying their bit values directly, which keeps tutorial and free-view camera
input working on the custom 0.7 client.

Full gameplay is tested with the client and server repositories linked above.
Generic clients do not implement the MRPG account window, HUD or custom visual
systems and are therefore not a complete replacement.

## Server features

- Account registration and login backed by MariaDB.
- Multi-world loading from the `tw_worlds` database table.
- Quests, NPCs, monsters, dungeons, professions, skills and equipment.
- Inventory, crafting, trading, auction, guild and warehouse systems.
- MRPG HUD data, broadcasts, vote menus and server-side visual entities.
- Map download and snapshot conversion for the paired legacy 0.7 client.
- Native 0.6 networking retained alongside the custom 0.7 bridge.

## Build on Linux

### Dependencies

Debian/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential cmake git python3 \
  libmariadb-dev libsqlite3-dev libcurl4-openssl-dev libssl-dev zlib1g-dev
```

Arch Linux:

```bash
sudo pacman -S --needed base-devel cmake git python mariadb-libs sqlite \
  curl openssl zlib
```

Clone and build:

```bash
git clone --recurse-submodules https://github.com/sqwinixxx/Teeworlds-MRPG-Server.git
cd Teeworlds-MRPG-Server
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target mmoteeworlds_srv -j4
```

If the repository was cloned without submodules:

```bash
git submodule update --init --recursive
```

## Database setup

MariaDB is required for worlds, accounts and all persistent MRPG data. The
included `mmprpg_clean.sql` file contains the schema and initial game data.

Example setup:

```bash
sudo mariadb
```

```sql
CREATE DATABASE mrpg CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE USER 'mrpg'@'localhost' IDENTIFIED BY 'change_this_password';
GRANT ALL PRIVILEGES ON mrpg.* TO 'mrpg'@'localhost';
FLUSH PRIVILEGES;
EXIT;
```

Import the database:

```bash
mariadb -u mrpg -p mrpg < mmprpg_clean.sql
```

Use a unique database password in production. Do not commit it to the
repository.

## Configuration

Create `autoexec_server.cfg` in the repository root. It is intentionally
ignored by Git so database and RCON passwords are not published.

Minimal example:

```cfg
sv_name "Teeworlds MRPG"
sv_port 8310
sv_register 0
sv_map main
sv_gamemode_name "MRPG"

# Required for the paired custom 0.7 client.
sv_sixup 1

# Leave empty unless every player should enter a connection password before
# the separate MRPG account login/register screen.
password ""

sv_rcon_password "replace_with_a_long_random_password"

sv_sql_host "127.0.0.1"
sv_sql_port 3306
sv_sql_database "mrpg"
sv_sql_login "mrpg"
sv_sql_password "change_this_password"
sv_sql_pool_size 3
sv_sql_use_tls 0
```

Important settings:

- `sv_sixup 1` enables connections from the paired custom 0.7 client.
- `password` is the server connection password, not an MRPG account password.
- `sv_register 0` keeps a development server private. Configure registration and firewall/NAT rules before publishing it.
- `sv_map main` selects the initial map; additional worlds are loaded from the database.
- `sv_sql_*` must match the MariaDB database and user created above.

## Running

Start the binary from the repository root. This working directory is required
for maps, `server_data`, language files and configuration files to resolve
correctly.

```bash
./build/mmoteeworlds_srv
```

Players can then connect with the paired client to `SERVER_IP:8310`, or to the
port selected in `sv_port`.

## Account flow

The paired client shows the rules and account window after joining:

1. A new player accepts the rules and registers a login/password.
2. An existing player enters the same login/password and selects **Join**.
3. Account credentials are read from MariaDB and are separate from `password` in the server config.

If the client asks for a password before displaying the MRPG account window,
the server-level `password` setting is not empty.

## Troubleshooting

- **Custom 0.7 client cannot connect:** confirm this server build is running and `sv_sixup 1` is set.
- **Map download fails:** run the server from the repository root and verify the required `.map` files exist in `maps/`.
- **Worlds were not initialized:** verify the MariaDB connection and import `mmprpg_clean.sql`; the server loads world definitions from `tw_worlds`.
- **Account password is incorrect:** confirm the player is using the registered MRPG password, not the server connection or RCON password.
- **Client camera is locked in tutorial/free-view:** update both repositories to the same release; the compatibility bridge must translate game-state flags.
- **SQL connection errors:** check `sv_sql_host`, port, database, user grants and whether MariaDB is running.

## Releasing compatible builds

Tag the client and server with the same version, for example `compat-v0.1.0`.
The two tags then identify a tested pair. Publish client binaries in the client
repository and server binaries or deployment notes in this repository. Never
include production database, RCON or server passwords in release archives.

## Related repository

Required custom client:
[sqwinixxx/Teeworlds-MRPG-Client](https://github.com/sqwinixxx/Teeworlds-MRPG-Client).

The original 0.6 MRPG maintenance wiki is available in the
[upstream repository](https://github.com/kurosio/Teeworlds-MRPG-0.6/wiki).

## License and credits

This project is based on Teeworlds, DDNet and the original MRPG server. See
[`license.txt`](license.txt) for license and copyright information. Originally
written by Magnus Auvinen and extended by the Teeworlds, DDNet and MRPG
contributors.
