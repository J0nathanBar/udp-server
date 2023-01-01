import { getValue } from '@testing-library/user-event/dist/utils';
import React from 'react';
import { useRef } from 'react';

import { TextForm } from '../../components';


const Transmitter = (props) => {
  const inputRef = useRef(null);

  return (
    <div>
      <h1>Transmitter</h1>
      <TextForm name="TransConf.json" />
    </div>

  )
}



export default Transmitter