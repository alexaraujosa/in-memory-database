const fsp = require("fs/promises");
const path = require("path");
const util = require("util")
// const jsonc = require("jsonc-parser");
const jsonc = require("jsonc");
const exec = util.promisify(require('child_process').exec);

const vscodePath = path.join(__dirname, "../../.vscode/");

;(async () => {
    try {
        const propFilePath = path.join(vscodePath, "c_cpp_properties.json");
        const propFileData = await fsp.readFile(propFilePath, { encoding: "utf8" });
        const propData = jsonc.parse(propFileData);

        const { stdout, stderr } = await exec('pkg-config --cflags glib-2.0');
        if (stderr) {
            console.error('Error running pkg-config:', stderr);
            return;
        }
        const glibLibs = stdout.trim();

        const conf = propData.configurations.filter(c => c.name === "Linux")[0];
        conf.includePath.push(...glibLibs.split(/(?: )?\-I/).filter(l => !!l));

        await fsp.writeFile(propFilePath, jsonc.stringify(propData, null, 4), { encoding: "utf8" });
        console.log("Successfully wrote C/Cpp Property File.");
    } catch (e) {
        console.error('Could not update C/Cpp Property file:', e);
    }
})();