const fsp = require("fs/promises");
const path = require("path");

(async () => {
    const filePath = path.join(__dirname, "./stats.out");
    const newFilePath = path.join(__dirname, "./stats_corrected.out");

    const fileData = await fsp.readFile(filePath, { encoding: "utf8" });
    const newFileData = fileData.replaceAll(".", ",");
    await fsp.writeFile(newFilePath, newFileData, { encoding: "utf8" });
})();