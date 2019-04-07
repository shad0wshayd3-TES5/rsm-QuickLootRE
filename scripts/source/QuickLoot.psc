ScriptName QuickLoot


; @brief Registers the given form for the OnContainerOpenAnim event.
; @param a_form - The form to register.
Function RegisterForContainerOpenAnimEvent(Form a_form) Global Native


; @brief Unregisters the given form for the OnContainerOpenAnim event.
; @param a_form - The form to unregister.
Function UnregisterForContainerOpenAnimEvent(Form a_form) Global Native


; @brief Registers the given form for the OnContainerCloseAnim event.
; @param a_form - The form to register.
Function RegisterForContainerCloseAnimEvent(Form a_form) Global Native


; @brief Unregisters the given form for the OnContainerCloseAnim event.
; @param a_form - The form to unregister.
Function UnregisterForContainerCloseAnimEvent(Form a_form) Global Native


; @brief Called when QuickLoot executes a container's open animation.
Event OnContainerOpenAnim()
EndEvent


; @brief Called when QuickLoot executes a container's close animation.
Event OnContainerCloseAnim()
EndEvent
