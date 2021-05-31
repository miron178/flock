#include "Scene.h"

int main()
{
	Scene* pScene = Scene::GetInstance();
	if (pScene)
	{
		bool bShouldQuit = pScene->Initialise();
		if (bShouldQuit)
		{
			bool bKeepRunning = false;
			while (!bKeepRunning)
			{
				bKeepRunning = pScene->Update();
				pScene->Render();
			}
			pScene->Deinitialise();
		}
	}
	delete pScene;

	return 0;
}
