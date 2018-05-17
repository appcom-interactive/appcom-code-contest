https://rust-lang-nursery.github.io/rust-wasm/hello-world.html

cargo new philip --bin
cargo run
cargo +nightly build --target wasm32-unknown-unknown --release

https://stackoverflow.com/questions/47529643/how-to-return-a-string-or-similar-from-rust-in-webassembly?noredirect=1&lq=1&utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa


https://github.com/hunterlester/rusty_safe_web
rustup target add wasm32-unknown-emscripten
cargo build --target=wasm32-unknown-emscripten --release

----------------

cargo build --target=wasm32-unknown-emscripten --release
cargo web start --target=wasm32-unknown-emscripten --release