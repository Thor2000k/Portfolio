package cp;

import java.nio.file.Path;

/*
This class implements the Result interface and is used for making objects of the type Result
 */


public class ResultClass implements Result {
    private final Path resPath;
    private final int resNum;

    public ResultClass(Path resPath, int resNum){

        this.resPath = resPath;
        this.resNum = resNum;
    }




    public Path path() {
        return resPath;
    }


    public int number() {
        return resNum;
    }
}
