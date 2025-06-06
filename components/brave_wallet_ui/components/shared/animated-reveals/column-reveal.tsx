// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import {
  RevealableContentContainer,
  RevealableContentColumn,
} from './column-reveal.styles'

interface Props {
  hideContent?: boolean
}

export const ColumnReveal: React.FC<React.PropsWithChildren<Props>> = ({
  children,
  hideContent,
}) => {
  return (
    <RevealableContentContainer hideContent={hideContent}>
      <RevealableContentColumn>{children}</RevealableContentColumn>
    </RevealableContentContainer>
  )
}

export default ColumnReveal
