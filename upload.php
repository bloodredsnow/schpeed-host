<?php

$targetDir = "/var/www/erm.cool/files/endpoint/";
$uploadOk = 1;

error_reporting(E_ALL);
ini_set('display_errors', 1);

echo "Submitting file... <br>";

if ($_FILES["file"]["error"] == UPLOAD_ERR_OK) {

    if ($_FILES["file"]["size"] > 5000000) { 
        echo "Sorry, your file is too large. <br>";
        $uploadOk = 0;
    }

    $fileType = strtolower(pathinfo($_FILES["file"]["name"], PATHINFO_EXTENSION));
    $allowedTypes = ['jpg', 'png', 'jpeg', 'gif', 'pdf', 'txt'];
    if (!in_array($fileType, $allowedTypes)) {
        echo "Sorry, only JPG, JPEG, PNG, GIF, PDF, & TXT files are allowed.";
        $uploadOk = 0;
    }

    if ($uploadOk == 0) {
        echo "Sorry, your file was not uploaded.<br>";
    } else {
        // Generate a unique hash for the file name
        $hash = hash('sha256', uniqid(rand(), true));
        $hashedFileName = $hash . '.' . $fileType; 

        $targetFile = $targetDir . $hashedFileName;

        if (move_uploaded_file($_FILES["file"]["tmp_name"], $targetFile)) {

	    echo "Upload finished! <br>";

	    echo "Your file was uploaded! Please allow up to 2 nano seconds for my based cool swag feet omg c backend to create your website under the following link: <br>";
	    echo "<br>";

	    $fullpath = "https://erm.cool/files/endpoint/websites/" . htmlspecialchars($hash) . "/";

	    echo '<a href="#" onclick="navigator.clipboard.writeText(\'' . $fullpath . '\'); alert(\'Link copied to clipboard: ' . $fullpath . '\'); return false;">click to copy</a>';

	    } else {

	    echo "Sorry, there was an error uploading your file. <br>";
        }
    }

} else {
    echo "Error: " . $_FILES["file"]["error"];
}

?>
