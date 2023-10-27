# UMD Region Changer
##### Presented by ARK-4 Team

<br />

### Compiling

1. `make` use ARK-4 SDK

2. After compiling the files will be in `dist`

3. After copying `UMD_Region_Changer` to `ms0:/PSP/GAME` make sure your UMD door is NOT closed/detected then run it.

4. If you accidentally do have your UMD door closed, once the XMB is loaded you can just open and close the UMD door and the UMD will be allowed to be played.


<p>Note:As of right now this does not support Game Categories Lite. I will be working on that to see if I can figure a way to support it. Thanks for your cooperation and patience. 


### Releases

For the latest release go <a href="https://github.com/PSP-Archive/UMD_Region_Changer/releases/latest">here</a>


## Setup & Install


#### PRO Users

1. Run UMD Region Changer

2. Choose the region

3. Once XMB display insert UMD of you selected Region

4. Play and enjoy your Movie 



#### (L)ME Users

<p>Reason why ME is forced to use a plugin. For some reason <code>sctrlHENLoadModuleOnReboot</code> does not play nice with ME.
I had to create a plugin that would do the equivalent that the normal EBOOT does but as a plugin for it to work on ME.
Like no joke its almost a 1:1 copy and paste of it.
<code>sctrlHENLoadModuleOnReboot</code> is slightly different than on PRO and ARK from reading the documentation for instance,
the first argument on PRO and ARK-4 is module_before, but with ME its module_after so even if it "just worked" the placement
of the patches would not correlate with what we thought was going to happen. It seems that that only works via game interaction or some sorts.
Which is very bizarre as its part of systemctrl so it should be available globally? I will be more than happy to discuss my findings
or if they are in need so some test cases. I was determined to find a way to get it working and did, not 100% ideal by all means, but it does work.</p>

1. copy `UMD_Region_Changer_ME_fix.prx` to `SEPLUGINS` located on your PSP

2. Add `ms0:/SEPLUGINS/UMD_Region_Changer_ME_fix.prx 1` to `GAME.TXT`

3. Run UMD Region Changer

4. Choose the region

5. Once XMB display insert UMD of you selected Region

6. Play and enjoy your Movie 


<hr/>


<p><b>NOTE:</b> This is not persistent, if you restart your PSP the UMD Drive will default back to its original state.</p>


### Credits:
<p><b><u>ARK-4 TEAM</u></b>
 
- Acid_Snake for helping me port this over from ARK-4

</p>

<p><b><u>Others</u></b>

 - PSP_Cult for providing the graphics and logo(s)/icon(s) and testing.
 - Github Search with other users provided me to get ME support working, such as `qwikrazor87`'s npdrm_free repo. May he rest in peace, I truly wish you were hear to take guidance from.

</p>


<span>ARK-4 Team &#169;2023</span>
