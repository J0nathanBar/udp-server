import React, { useState } from 'react';
import { useDropzone } from 'react-dropzone';
//import './FileSelector.css';

function FileSelector({ selectedFile, setSelectedFile }) {
  const [isDragActive, setIsDragActive] = useState(false);
  const { getRootProps, getInputProps, isDragAccept } = useDropzone({
    accept: '*',
    onDrop: (acceptedFiles) => {
      setSelectedFile(acceptedFiles[0]);
    },
    onDragEnter: () => {
      setIsDragActive(true);
    },
    onDragLeave: () => {
      setIsDragActive(false);
    }
  });

  return (
    <div className="dropzone-container">
      <div {...getRootProps()} className={`dropzone ${isDragActive ? 'active' : ''} ${isDragAccept ? 'accept' : ''}`}>
        <input {...getInputProps()} />
        {selectedFile ? (
          <div className="selected-file">
            {selectedFile.path}
          </div>
        ) : (
          <div className="prompt">
            Drag and drop a file or directory here, or click to select a file
          </div>
        )}
      </div>
    </div>
  );
}

export default FileSelector;
