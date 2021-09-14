import time,pyautogui

start = time.time()
flag = 0
while(1):
    autoPos = pyautogui.locateCenterOnScreen("./image/auto.png", grayscale=True)
    stageDecidePos = pyautogui.locateCenterOnScreen("./image/stage_decide.png", grayscale=True)
    stageStartPos = pyautogui.locateCenterOnScreen("./image/stage_start.png", grayscale=True)
    resultPos = pyautogui.locateCenterOnScreen("./image/result.png", grayscale=True)
    if autoPos is not None:
        pyautogui.click(stageDecidePos)
        print("click stageDecide")
        flag = 0
    elif stageStartPos is not None:
        pyautogui.click(stageStartPos)
        print("click stageStart")
        time.sleep(90)
        flag = 0
    elif resultPos is not None:
        pyautogui.click(resultPos)
        print("click result")
        flag = 0
    # elif sanityPos is not None:
    #     pyautogui.click(yesPos[0],yesPos[1])
    #     print("used sanity_potion")
    else:
        flag += 1
        if flag > 30:
            break
    time.sleep(5)
    print(str(round(time.time()-start,3))+"秒経過")
