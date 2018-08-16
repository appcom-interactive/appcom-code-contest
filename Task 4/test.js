// Replace `template` with your own solution
const fs = require("fs");
const path = require("path");
const mocha = require("mocha");
const expect = require("chai").expect;

const it = mocha.it;
const describe = mocha.describe;

const files = fs
  .readdirSync(path.resolve(__dirname, "./solutions"))
  .filter(
    file =>
      file !== "template.js" &&
      file !== ".DS_Store" &&
      !fs.lstatSync(path.resolve(__dirname, "./solutions", file)).isDirectory()
  )
  .map(file => ({
    name: file.replace(".js", ""),
    impl: require(path.resolve("./solutions", file))
  }));

files.forEach(({ name, impl }) => {
  describe(`${name} solution`, () => {
    it("should return Player A wins", () => {
      expect(
        impl.checkGame([
          [null, null, null, null, null, null, null],
          [null, null, null, null, null, null, null],
          [null, null, null, null, "A", null, null],
          [null, "B", "B", "A", "B", null, null],
          [null, "A", "A", "B", "A", "B", null],
          [null, "A", "B", "A", "A", "B", null]
        ])
      ).to.eq("Player A wins!");
    });

    it("should return Player A wins", () => {
      expect(
        impl.checkGame([
          [null, null, null, null, null, null, null],
          [null, null, null, null, null, null, null],
          [null, null, null, null, "A", null, null],
          [null, "B", "B", null, "A", "B", null],
          [null, "A", "A", "B", "A", "B", null],
          [null, "A", "B", "A", "A", "B", null]
        ])
      ).to.eq("Player A wins!");
    });

    it("should return Player A wins", () => {
      expect(
        impl.checkGame([
          [null, null, null, null, null, null, null],
          [null, null, null, "B", "B", null, null],
          [null, null, "B", "A", "A", "A", "A"],
          ["A", "B", "B", "A", "B", "A", "B"],
          ["B", "A", "A", "B", "A", "B", "A"],
          ["A", "A", "B", "A", "B", "B", "B"]
        ])
      ).to.eq("Player A wins!");
    });

    it("should return Player B wins", () => {
      expect(
        impl.checkGame([
          [null, null, null, null, null, null, null],
          [null, null, null, "B", null, null, null],
          [null, null, "B", "A", null, null, null],
          ["A", "B", "B", "A", "A", "B", null],
          ["B", "A", "A", "B", "A", "B", null],
          ["A", "A", "B", "A", "A", "B", null]
        ])
      ).to.eq("Player B wins!");
    });

    it("should return error because finger in hole", () => {
      expect(
        impl.checkGame([
          [null, null, null, null, null, null, null],
          [null, "A", null, null, null, null, null],
          [null, null, null, null, null, null, null],
          [null, "B", null, null, null, null, null],
          [null, "B", null, null, null, null, null],
          [null, "A", "A", null, null, null, null]
        ])
      ).to.eq("Cheater! Cheater! Cheater!");
    });

    it("should return error because double coin toss", () => {
      expect(
        impl.checkGame([
          [null, null, null, null, null, null, null],
          [null, null, null, null, null, null, null],
          [null, null, null, null, null, null, null],
          [null, "B", "B", "B", null, null, null],
          [null, "A", "A", "B", "A", "B", null],
          [null, "A", "B", "A", "A", "B", null]
        ])
      ).to.eq("Cheater! Cheater! Cheater!");
    });

    it("should return error because wrong coin", () => {
      expect(
        impl.checkGame([
          [null, null, null, null, null, null, null],
          [null, null, null, null, null, null, null],
          [null, null, null, null, null, null, null],
          [null, "B", "B", "C", null, null, null],
          [null, "A", "A", "B", "A", "B", null],
          [null, "A", "B", "A", "A", "B", null]
        ])
      ).to.eq("Go away! Strange kid!");
    });
  });
});
