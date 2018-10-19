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
    it("should return X:4, Z:2)", () => {
      expect(
        impl.drillCheck([
          [
            ['E', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E'],
          ],
          [
            ['E', 'E', 'E', 'E', 'E'],
            ['E', 'G', 'G', 'E', 'S'],
            ['E', 'G', 'G', 'K', 'S'],
            ['S', 'E', 'K', 'K', 'S'],
            ['E', 'S', 'E', 'S', 'E'],
          ],
          [
            ['H', 'E', 'E', 'E', 'E'],
            ['E', 'G', 'G', 'E', 'S'],
            ['E', 'G', 'H', 'K', 'S'],
            ['K', 'E', 'K', 'H', 'S'],
            ['E', 'E', 'E', 'E', 'E'],
          ],
          [
            ['K', 'E', 'S', 'E', 'E'],
            ['E', 'G', 'G', 'E', 'S'],
            ['E', 'G', 'H', 'G', 'E'],
            ['K', 'E', 'E', 'K', 'S'],
            ['E', 'S', 'E', 'E', 'E'],
          ],
          [
            ['G', 'E', 'S', 'E', 'E'],
            ['E', 'K', 'K', 'E', 'S'],
            ['E', 'G', 'H', 'G', 'E'],
            ['K', 'E', 'E', 'K', 'S'],
            ['E', 'E', 'S', 'G', 'G'],
          ]
        ])
      ).to.deep.equal({x: 4, z: 2, t: 6});
    });
    it("should return x:2, z:5 and x:4, z:3)", () => {
      expect(
        impl.drillCheck([
          [
            ['S', 'E', 'E', 'E', 'E', 'K'],
            ['E', 'H', 'E', 'S', 'E', 'E'],
            ['E', 'E', 'S', 'E', 'E', 'E'],
            ['E', 'S', 'E', 'H', 'E', 'E'],
            ['E', 'E', 'L', 'L', 'S', 'E'],
            ['K', 'E', 'L', 'L', 'E', 'S'],
          ],
          [
            ['E', 'S', 'S', 'S', 'S', 'S'],
            ['S', 'K', 'E', 'E', 'S', 'S'],
            ['S', 'E', 'E', 'E', 'S', 'S'],
            ['S', 'E', 'E', 'K', 'S', 'S'],
            ['S', 'E', 'E', 'E', 'E', 'S'],
            ['S', 'S', 'S', 'S', 'S', 'E'],
          ],
          [
            ['L', 'E', 'S', 'E', 'E', 'G'],
            ['S', 'K', 'S', 'L', 'E', 'E'],
            ['L', 'E', 'L', 'L', 'S', 'S'],
            ['E', 'E', 'E', 'K', 'E', 'E'],
            ['S', 'L', 'S', 'E', 'L', 'E'],
            ['L', 'E', 'G', 'E', 'E', 'K'],
          ],
          [
            ['G', 'G', 'L', 'L', 'S', 'K'],
            ['G', 'L', 'L', 'L', 'S', 'S'],
            ['G', 'G', 'L', 'L', 'S', 'S'],
            ['E', 'S', 'E', 'L', 'E', 'S'],
            ['E', 'S', 'G', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E', 'E'],
          ],
          [
            ['E', 'E', 'L', 'E', 'E', 'G'],
            ['S', 'H', 'S', 'E', 'L', 'E'],
            ['E', 'S', 'E', 'E', 'E', 'E'],
            ['S', 'E', 'S', 'H', 'S', 'K'],
            ['E', 'S', 'E', 'E', 'E', 'K'],
            ['S', 'E', 'S', 'E', 'S', 'E'],
          ],
          [
            ['H', 'S', 'S', 'E', 'E', 'E'],
            ['H', 'K', 'S', 'L', 'S', 'E'],
            ['H', 'S', 'S', 'E', 'S', 'L'],
            ['E', 'L', 'E', 'K', 'S', 'L'],
            ['S', 'L', 'K', 'E', 'E', 'E'],
            ['E', 'L', 'E', 'L', 'E', 'S'],
          ]
        ])
      ).to.deep.equal([{x: 2, z: 5, t: 5.5}, {x: 4, z: 3, t: 5.5}]);
    });
    it("should return 'Unwirtschaftliches Gelände'", () => {
      expect(
        impl.drillCheck([
          [
            ['E', 'E', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'E', 'E', 'E', 'E'],
          ],
          [
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
          ],
          [
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
            ['G', 'G', 'G', 'G', 'G', 'G'],
          ],
        ])
      ).to.eq('Unwirtschaftliches Gelände');
    });
    it("should return 'Unvollständige Geländedaten'", () => {
      expect(
        impl.drillCheck([
          [
            ['E', 'E', 'K', 'E', 'E', 'L'],
            ['K', 'L', 'S', 'E', 'L', 'E'],
            ['E', 'S', 'E', 'L', 'E', 'E'],
            ['S', 'E', 'L', 'L', 'E', 'S'],
            ['E', 'S', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'S', 'K', 'E', 'L'],
          ],
          [
            ['E', 'E', 'K', 'E', 'E', 'L'],
            ['K', 'S', 'S', 'E', 'L', 'E'],
            ['E', 'S', 'E', 'L', 'E', 'E'],
            ['S', 'E', ' ', 'S', 'E', 'S'],
            ['E', 'S', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'S', 'K', 'E', 'L'],
          ],
          [
            ['E', 'E', 'K', 'E', 'E', 'L'],
            ['K', ' ', 'S', 'E', 'L', 'E'],
            ['E', 'S', 'E', 'L', 'E', 'E'],
            ['S', 'E', 'L', ' ', 'E', 'S'],
            ['E', 'S', 'E', 'E', 'E', 'E'],
            ['E', 'E', 'S', 'K', 'E', 'L'],
          ]
        ])
      ).to.eq('Unvollständige Geländedaten');
    });
  });
});
