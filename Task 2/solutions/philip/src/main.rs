use std::collections::HashMap;
use std::ffi::CString;
use std::os::raw::c_char;

fn main() {
    println!("--- DECRYPT ---");
    decrypt("Rn0n3tonvo?", 1);
    decrypt("abCDefgh", 2);
    decrypt("appCoM", 2);
    decrypt("KäsE", 2);
    decrypt("KeKsDOSe", 1);
}

fn fchoice() -> HashMap<char, String> {
    let mut map = HashMap::new();

    map.insert('a', String::from("g"));
    map.insert('e', String::from("p"));
    map.insert('h', String::from("h"));
    map.insert('l', String::from("r"));
    map.insert('o', String::from("a"));
    map.insert('t', String::from("s"));
    map.insert('w', String::from("e"));

    map.insert('A', String::from("G"));
    map.insert('E', String::from("P"));
    map.insert('H', String::from("H"));
    map.insert('L', String::from("R"));
    map.insert('O', String::from("A"));
    map.insert('T', String::from("S"));
    map.insert('W', String::from("E"));

    return map;
}

fn schoice() -> HashMap<char, String> {
    let mut map = HashMap::new();

    map.insert('z', String::from("d"));
    map.insert('c', String::from("b"));
    map.insert('v', String::from("x"));
    map.insert('n', String::from("e"));
    map.insert('r', String::from("g"));
    map.insert('p', String::from("j"));
    map.insert('y', String::from("l"));
    map.insert('t', String::from("m"));
    map.insert('o', String::from("t"));

    map.insert('0', String::from("h"));
    map.insert('8', String::from("f"));
    map.insert('2', String::from("e"));
    map.insert('3', String::from("i"));

    map.insert('Z', String::from("D"));
    map.insert('C', String::from("B"));
    map.insert('V', String::from("X"));
    map.insert('N', String::from("E"));
    map.insert('R', String::from("G"));
    map.insert('P', String::from("J"));
    map.insert('Y', String::from("L"));
    map.insert('T', String::from("M"));
    map.insert('O', String::from("T"));

    return map;
}

fn tchoice() -> HashMap<char, String> {
    let mut map = HashMap::new();

    map.insert('0', String::from("c"));
    map.insert('1', String::from("b"));
    map.insert('2', String::from("s"));
    map.insert('3', String::from("j"));
    map.insert('4', String::from("0"));
    map.insert('5', String::from("p"));
    map.insert('6', String::from("u"));
    map.insert('7', String::from("f"));
    map.insert('8', String::from("4"));
    map.insert('9', String::from("a"));

    map.insert('a', String::from("7"));
    map.insert('b', String::from("6"));
    map.insert('c', String::from("q"));
    map.insert('d', String::from("d"));
    map.insert('e', String::from("r"));
    map.insert('f', String::from("e"));
    map.insert('g', String::from("t"));
    map.insert('h', String::from("2"));
    map.insert('i', String::from("n"));
    map.insert('j', String::from("3"));
    map.insert('k', String::from("l"));
    map.insert('l', String::from("1"));
    map.insert('m', String::from("9"));
    map.insert('n', String::from("k"));
    map.insert('o', String::from("g"));
    map.insert('p', String::from("m"));
    map.insert('q', String::from("y"));
    map.insert('r', String::from("5"));
    map.insert('s', String::from("o"));
    map.insert('t', String::from("i"));
    map.insert('u', String::from("w"));
    map.insert('v', String::from("z"));
    map.insert('w', String::from("8"));
    map.insert('x', String::from("x"));
    map.insert('y', String::from("h"));
    map.insert('z', String::from("v"));

    map.insert('A', String::from("7"));
    map.insert('B', String::from("6"));
    map.insert('C', String::from("Q"));
    map.insert('D', String::from("D"));
    map.insert('E', String::from("R"));
    map.insert('F', String::from("E"));
    map.insert('G', String::from("T"));
    map.insert('H', String::from("2"));
    map.insert('I', String::from("N"));
    map.insert('J', String::from("3"));
    map.insert('K', String::from("L"));
    map.insert('L', String::from("1"));
    map.insert('M', String::from("9"));
    map.insert('N', String::from("K"));
    map.insert('O', String::from("g"));
    map.insert('P', String::from("M"));
    map.insert('Q', String::from("Y"));
    map.insert('R', String::from("5"));
    map.insert('S', String::from("O"));
    map.insert('T', String::from("I"));
    map.insert('U', String::from("W"));
    map.insert('V', String::from("Z"));
    map.insert('W', String::from("8"));
    map.insert('X', String::from("X"));
    map.insert('Y', String::from("H"));
    map.insert('Z', String::from("V"));

    return map;
}

extern fn decrypt(value: &str, mapping_number: i16) -> *mut c_char {
    let mut new_string: String = "".to_owned();
    let mut choice = fchoice();

    if mapping_number == 1 {
        choice = schoice();
    } else if mapping_number == 2 {
        choice = tchoice();
    }

    // let mut new_string;
    for current_char in value.chars() {
        let val = choice.get(&current_char);

        if val != None {
            new_string.push_str(choice.get(&current_char).unwrap());
        } else {
            new_string.push_str(&current_char.to_string());
        }
    }

    println!("{:?}", new_string);

    return  CString::new(new_string.as_str()).unwrap().into_raw();
}
